#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/export.h>
#include <asm/uaccess.h>
#include <linux/device.h>
#include <asm/io.h>
#include <asm/page.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/gpio/consumer.h>

#include "load.h"

#define BUF_LEN 128
#define LED_CHRDEV_NAME "zlj_led"
#define LED_DEV_NAME "zljled"
#define LED_DRV_CLASS_NAME "zlj_led_drv"
#define LED_DEVICE_NAME "zlj_led_drv_"
#define DT_VERSION "version"
#define DT_IDS_COMPATIBLE "zlj_led_drv"
#define MIN_INTERVAL 100
#define TIMES_INTERVAL(n) (ms_to_ktime(MIN_INTERVAL * n))

static int major;
static struct class* led_drv_class;
static struct gpio_desc* led_gpio;

static char led_ver_buf[BUF_LEN];
static struct hrtimer on_timer;
static struct hrtimer off_timer;

static int led_counter = 1;
static int led_step = -1;


/* enable gpio
 * config pin as gpio
 * config gpio as output 
 */
static int led_fpos_open(struct inode *inode, struct file *file)
{
	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	/* 1. gpio module enabled defualtly */
	/* 2. already configured as gpio mode in device tree & pinctrl */
	/* 3. already get gpio pin in file_ops.probe: led_gpio = gpiod_get(&pdev->dev, "led", 0); */
	
	/* 4. config gpio as output */
	gpiod_direction_output(led_gpio, 0);
	
	return 0;
}

static int led_fops_release(struct inode *inode, struct file *file)
{
	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}

static enum hrtimer_restart off_timer_cb(struct hrtimer* hrtimer)
{
	enum hrtimer_restart ret = HRTIMER_NORESTART;

	gpiod_set_value(led_gpio, 0);	// 熄灭

    // 更新闪烁周期
    if (led_counter == 1 || led_counter == 10)
        led_step *= -1;
    led_counter += led_step;

	// 设置下一次点亮
    hrtimer_start(&on_timer, TIMES_INTERVAL(led_counter), HRTIMER_MODE_REL);
	
    return ret;

}

static enum hrtimer_restart on_timer_cb(struct hrtimer *hrtimer)
{
	enum hrtimer_restart ret = HRTIMER_NORESTART;

	gpiod_set_value(led_gpio, 1); // 点亮

	// 安排 100ms 后熄灭
    hrtimer_start(&off_timer, ms_to_ktime(100), HRTIMER_MODE_REL);
	
	return ret;
}

static void led_init_timer(void)
{
    if (!on_timer.function) {  // 仅在 function 未赋值时初始化
        hrtimer_init(&on_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
        on_timer.function = on_timer_cb;
    }

	if (!off_timer.function) {
		hrtimer_init(&off_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
		off_timer.function = off_timer_cb;
	}
}

static void led_start_timer(void)
{
	led_init_timer();

	// if (hrtimer_active(&timer))	// 第一次调用write时，会因为timer没有初值，造成错误：Unable to handle kernel NULL pointer dereference at virtual address 00000000
        // return;  // 避免重复启动
	if (hrtimer_is_queued(&on_timer)) 
		return;

	// 以点亮为起始操作，牵动熄灭操作; 熄灭操作起始也没问题，会多一步无用熄灭操作;
	hrtimer_start_range_ns(&on_timer, TIMES_INTERVAL(30), 0, HRTIMER_MODE_REL);
}

static void led_cancel_timer(void)
{
	if (on_timer.function && hrtimer_active(&on_timer)) {

		printk("hrtimer_cancel in %s %d %s\n", __FUNCTION__, __LINE__, __FILE__);
		hrtimer_cancel(&on_timer);
	}
		
	if (off_timer.function && hrtimer_active(&off_timer)) {

		printk("hrtimer_cancel in %s %d %s\n", __FUNCTION__, __LINE__, __FILE__);
		hrtimer_cancel(&off_timer);
	}
	
}

static ssize_t led_fpos_write(struct file *file, const char __user *buf,
			 size_t count, loff_t *ppos)
{
	unsigned int err;
	char val;
	
	/* copy_from user : get data from APP */
	err = copy_from_user(&val, buf, 1);
	if (err > 0)
	{
		printk("copy_from_user failed, uncopied character count(er)= %du\n", err);
		return -EFAULT;
	}
	
	/* to set gpio data register : output 1/0 */
	if (val)
	{
		led_start_timer();
		printk("start timer!!!\n");
	}
	else
	{
		led_cancel_timer();
		printk("end timer!!!\n");
		
		/* set gpio to let led off */
		gpiod_set_value(led_gpio, 0);
	}

	return err;
}

static const struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.write = led_fpos_write,
	.open = led_fpos_open,
	.release = led_fops_release,
};

static int led_probe(struct platform_device *pdev)
{
	int len;
	int err;
	const char* vers;

	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

	/* 4.1 设备树中定义有: led-gpios=<...>; */
	/* gpiod_get 第 3 个参数，probe 里面并不指定使用哪个引脚，因为 probe 时并不会使用到引脚，用到时再设置 */
	led_gpio = gpiod_get(&pdev->dev, "led", 0);
	if (IS_ERR(led_gpio)) {
		dev_err(&pdev->dev, "Failed to get GPIO for led\n");
		return PTR_ERR(led_gpio);
	}
	
	/* 从匹配的platform_device 当中得到版本信息 */
	err = of_property_read_string_index(pdev->dev.of_node, DT_VERSION, 0, &vers);
	if (err < 0)
		vers = "no version";
	len = strlen(vers) + 1;
	if (len > BUF_LEN)
		len = BUF_LEN;
	strncpy(led_ver_buf, vers, len);
	led_ver_buf[len - 1] = '\0';
	printk("Got led device version: %s\n", led_ver_buf);

	/* 注册led的字符驱动程序 */
  	// static inline int register_chrdev(unsigned int major, const char *name, const struct file_operations *fops)
	major = register_chrdev(0, LED_CHRDEV_NAME, &led_fops);
	if (major <= 0)
	{
		printk("register error.\n");
		return major;
	}
	else
	{
		printk("Got register_chrdev major: %d\n", major);
	}

	/* 创建led的设备节点 */
	led_drv_class = class_create(THIS_MODULE, LED_DRV_CLASS_NAME);
	if (IS_ERR(led_drv_class))
		return PTR_ERR(led_drv_class);
	device_create(led_drv_class, NULL, MKDEV(major, 0), NULL, LED_DEV_NAME);

	return 0;
	
}

static int led_remove(struct platform_device *pdev)
{
	led_cancel_timer();

	device_destroy(led_drv_class, MKDEV(major, 0));
	class_destroy(led_drv_class);

	/* 注销字符驱动程序 */
	unregister_chrdev(major, LED_CHRDEV_NAME);
	gpiod_put(led_gpio);

	return 0;
}

static const struct of_device_id led_dt_ids[] = {
	{ .compatible = DT_IDS_COMPATIBLE, },
	{ /* sentinel */ }
};

static struct platform_driver led_driver = {
	.driver = {
		.name = LED_DEVICE_NAME,
		.of_match_table = led_dt_ids,
	},
	.probe = led_probe,
	.remove = led_remove,
};
		
static int __init led_drv_init(void)
{
	int err;
	err = platform_driver_register(&led_driver);
	printk(">>>>>>>>>> led platform_driver registered, ended! <<<<<<\n");
	printk("%s %s led_drv module loaded. \n", __FILE__, __FUNCTION__);
	return err;
}

static void __exit led_drv_exit(void)
{
	platform_driver_unregister(&led_driver);
	printk(">>>>>>>>>> led platform_driver unregistered, ended! <<<<<<\n");
	printk("%s %s led_drv module unloaded. \n", __FILE__, __FUNCTION__);
}

module_init(led_drv_init);
module_exit(led_drv_exit);

MODULE_LICENSE("GPL");
