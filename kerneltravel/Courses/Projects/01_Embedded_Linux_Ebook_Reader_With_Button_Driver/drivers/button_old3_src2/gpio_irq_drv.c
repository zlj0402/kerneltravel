#include <linux/module.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/irqreturn.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/poll.h>
// jiffies
#include <linux/jiffies.h>
// hrtimer
#include <linux/timer.h>

#include "load.h"

#define CHRDEV_NAME "reader_button"
#define TIMEOUT 50

struct gpio_key {
	
	int idx;
	int gpio;
	struct gpio_desc* gpiod;
	int irq;
	enum of_gpio_flags flag;
};

//static struct fasync_struct *button_fasync;

static struct gpio_key* gpio_keys;
/* 主设备号 */
static int major;
static struct class* gpio_key_class;

static wait_queue_head_t gpio_key_wait;
static DECLARE_WAIT_QUEUE_HEAD(gpio_key_wait);

static int g_pending_keys;
static unsigned long last_irq_time;
static struct hrtimer key_timer;

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> round robin queue <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

/* 循环队列 */
#define MAXGKEYS 128
#define NEXT_POS(x) ((x + 1) % MAXGKEYS) 
static int g_keys[MAXGKEYS];
static int f, r;

static bool is_key_buf_empty(void) {
	return f == r;
}

static bool is_key_buf_full(void) {
	return f == NEXT_POS(r);
}

static void put_key(int key) {

	if (!is_key_buf_full()) {
		
		g_keys[r] = key;
		r = NEXT_POS(r);
	}
}

static int get_key(void) {

	int key = 0;
	if (!is_key_buf_empty()) {
		
		key = g_keys[f];
		f = NEXT_POS(f);
	}
	return key;
}

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> hrtimer <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 定时器回调函数
static enum hrtimer_restart key_timer_callback(struct hrtimer* timer) {
	
	// 检查是否是两键同时
    if (g_pending_keys == 0b11) {   // 比如两个键 idx=0 和 idx=1
        printk("Two keys pressed, quit\n");
        put_key(3); // 假设 val=3 表示退出
    } else {
        // 单键处理
        if (g_pending_keys & 0b01)
            put_key(1);
        if (g_pending_keys & 0b10)
            put_key(2);
    }
	
	g_pending_keys = 0; // 清空等待
	
    return HRTIMER_NORESTART; // 返回此值表示周期运行
}

static void start_timer(void) {

    if (!key_timer.function) {  // 仅在 function 未赋值时初始化
        hrtimer_init(&key_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL_PINNED);
        key_timer.function = key_timer_callback;
    }

	//if (!hrtimer_is_queued(&key_timer))	// 仅在第一个按键来临时，启动定时器; 非每一个按键都启动，50 ms 内不断来，会覆盖;
	if (!hrtimer_active(&key_timer))
		hrtimer_start(&key_timer, ms_to_ktime(TIMEOUT), HRTIMER_MODE_REL_PINNED);
}

static void stop_timer(void) {
	
	if (hrtimer_active(&key_timer))
		hrtimer_cancel(&key_timer);
}

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> file_operations & fops.read <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

ssize_t gpio_key_drv_read(struct file *file, char __user *buf, size_t size, loff_t *off) {

	int err;
	int key;
	
	wait_event_interruptible(gpio_key_wait, !is_key_buf_empty());
	key = get_key();
	err = copy_to_user(buf, &key, sizeof(int));

	return sizeof(int);
}

static unsigned int gpio_key_drv_poll(struct file *fp, poll_table * wait) {

	poll_wait(fp, &gpio_key_wait, wait);

	return is_key_buf_empty() ? 0 : POLLIN | POLLRDNORM;
}

/*
static int gpio_key_drv_fasync(int fd, struct file *file, int on) {

	if (fasync_helper(fd, file, on, &button_fasync) >= 0)
		return 0;
	else 
		return -EIO;
}
*/

static struct file_operations gpio_key_drv = {
	
	.owner = THIS_MODULE,
	.read = gpio_key_drv_read,
	.poll = gpio_key_drv_poll,
	//.fasync = gpio_key_drv_fasync,
};

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> platform_driver & interrupt handler <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

static irqreturn_t gpio_key_irq_zlj(int irq, void* dev_id) {
	
	unsigned long now;
	struct gpio_key* gpio_key = dev_id;
	int val;
	
	val = gpiod_get_value(gpio_key->gpiod);
	
	// logic rasing 0 不处理
	if (val == 0)
		return IRQ_HANDLED;
	
	now = jiffies;
	
	// 记录当前按键
	g_pending_keys |= (1 << gpio_key->idx);
	
	start_timer();
	
	wake_up_interruptible(&gpio_key_wait);
	
	printk("interval: %lu jiffies\n", now - last_irq_time);
	last_irq_time = now;
	
	return IRQ_HANDLED;
}

static int gpio_irq_probe(struct platform_device *pdev)
{
	// 取出每一个 gpio
	// 转换成一个一个 中断号
	// 然后 request_irq

	struct device_node *node = pdev->dev.of_node; 
	int count;
	int i;
	enum of_gpio_flags flags;
	int gpio;
	int irq;
	int err;
	
	// static inline int of_gpio_count(struct device_node *np)
	count = of_gpio_count(node);
	if (!count) {

		printk("%s %s line %d, there isn't any gpio available\n", __FILE__, __FUNCTION__, __LINE__);
	} else {

		printk("gpio count: %d\n", count);
	}

	gpio_keys = kzalloc(count * sizeof(struct gpio_key), GFP_KERNEL);

	for (i = 0; i < count; i++) {

		gpio = of_get_gpio_flags(node, i, &flags);
		irq = gpio_to_irq(gpio);

		gpio_keys[i].idx = i;
		gpio_keys[i].gpio = gpio;
		gpio_keys[i].gpiod = gpio_to_desc(gpio);
		gpio_keys[i].irq = irq;
		gpio_keys[i].flag = flags;
		if (flags & OF_GPIO_ACTIVE_LOW) {
		    // 按键按下时电平为低，设置为低电平有效
    		flags |= GPIOF_ACTIVE_LOW;  // GPIOF_ACTIVE_LOW -> 在gpio系统中用来标识低电平有效
		}
		
  		err = devm_gpio_request_one(&pdev->dev, gpio, flags, NULL);
		if (err < 0) {
			dev_err(&pdev->dev, "Failed to request GPIO %d, error %d\n",
				gpio, err);
			return err;
		}

		printk("gpio %d irq %d\n", gpio, irq);

		// IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING -> 双边沿触发
		// request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags,
	    // 				const char *name, void *dev)
	    // name 不重要，随便写
	    // 中断触发时，dev 会作为参数，传给 handler 函数
		err = request_irq(irq, gpio_key_irq_zlj, IRQF_TRIGGER_FALLING, "zlj_gpio_keys", &gpio_keys[i]);
	}

	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);

	// ------------------- file_operations part ---------------------

	major = register_chrdev(0, CHRDEV_NAME, &gpio_key_drv);
	if (major <= 0)
	{
		printk("register error.\n");
		return major;
	}
	else
	{
		printk("Got register_chrdev major: %d\n", major);
	}
	
	gpio_key_class = class_create(THIS_MODULE, "zlj_gpio_key_class");
	if (IS_ERR(gpio_key_class))
		return PTR_ERR(gpio_key_class);
	device_create(gpio_key_class, NULL, MKDEV(major, 0), NULL, CHRDEV_NAME);

	return 0;
}

static int gpio_irq_remove(struct platform_device *pdev)
{
	struct device_node *node = pdev->dev.of_node; 
	int count;
	int i;

	count = of_gpio_count(node);

	for (i = 0; i < count; i++) {

		free_irq(gpio_keys[i].irq, &gpio_keys[i]);
	}
	
	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	
	stop_timer();

	// ---------------------- file_operations part -----------------------------

	device_destroy(gpio_key_class, MKDEV(major, 0));
	class_destroy(gpio_key_class);

	unregister_chrdev(major, CHRDEV_NAME);

	return 0;
}


const struct of_device_id gpio_irq_table[] = {
	{ .compatible = "button_driver,gpio_key" },
	{  }, 
};


static struct platform_driver gpio_irq_drv = {

	.probe = gpio_irq_probe,
	.remove = gpio_irq_remove,
	.driver = {
		.name = "zlj_gpio_key",
		.of_match_table = gpio_irq_table,
	},
};

static int __init gpio_irq_drv_init(void) {

	platform_driver_register(&gpio_irq_drv);
	printk(">>>>>>>>>> led platform_driver registered, ended! <<<<<<\n");
	printk("%s %s led_drv module loaded. \n", __FILE__, __FUNCTION__);
	return 0;
}

static void __exit gpio_irq_drv_exit(void) {

	platform_driver_unregister(&gpio_irq_drv);
	printk(">>>>>>>>>> led platform_driver unregistered, ended! <<<<<<\n");
	printk("%s %s led_drv module unloaded. \n", __FILE__, __FUNCTION__);
}

module_init(gpio_irq_drv_init);
module_exit(gpio_irq_drv_exit);

MODULE_LICENSE("GPL");
