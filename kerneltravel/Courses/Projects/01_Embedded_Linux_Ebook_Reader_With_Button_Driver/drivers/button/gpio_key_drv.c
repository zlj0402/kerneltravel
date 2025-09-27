// POLL_IN
#include <uapi/asm-generic/siginfo.h>
// irqreturn_t
#include <linux/irqreturn.h>
// gpiod_get_value
#include <linux/gpio/consumer.h>
// wake_up_interruptible
#include <linux/wait.h>
// kzalloc
#include <linux/slab.h>
// platform_device
#include <linux/platform_device.h>
// request_irq
#include <linux/interrupt.h>
// module_init...
#include <linux/module.h>

#include <button_opr.h>
#include <keys_queue.h>

#define MAXGKEYS 128
#define DRV_NAME "reader_button_driver"
#define DT_COMPAT "reader_button,gpio_key"

struct keys_queue* kque;
EXPORT_SYMBOL(kque);

static struct gpio_key* gpio_keys;

wait_queue_head_t gpio_key_wait;
EXPORT_SYMBOL(gpio_key_wait);
DECLARE_WAIT_QUEUE_HEAD(gpio_key_wait);

struct fasync_struct *button_fasync;
EXPORT_SYMBOL(button_fasync);

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> platform_driver & interrupt handler <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

static irqreturn_t reader_button_irq(int irq, void* dev_id) {

	struct gpio_key* gpio_key = dev_id;
	int val;
	int key;
	
	val = gpiod_get_value(gpio_key->gpiod);
	printk("%s %d key %d val %d\n", __FUNCTION__, __LINE__, irq, gpio_get_value(gpio_key->gpio));
	
	// 接收到不同的 button，区别被按下的键值
	key = (gpio_key->gpio << 8) | (val << 1);
	kque->put(kque, key);
	
	wake_up_interruptible(&gpio_key_wait);
	kill_fasync(&button_fasync, SIGIO, POLL_IN);
	
	return IRQ_HANDLED;
}

static int button_irq_probe(struct platform_device *pdev)
{
	// 取出每个 gpio，按规则转换成一个 中断号
	// 然后 request_irq
	int err;
	int irq;
	int gpio;

	enum of_gpio_flags flags;
	struct device_node *node = pdev->dev.of_node; 
	
	int i;
	int count;
	
	// static inline int of_gpio_count(struct device_node *np)
	count = of_gpio_count(node);
	if (!count) {
		printk("%s %s line %d, there isn't any gpio available\n", __FILE__, __FUNCTION__, __LINE__);
	} else {
		printk("%s %d gpio count: %d\n", __FUNCTION__, __LINE__, count);
	}

	gpio_keys = kzalloc(count * sizeof(struct gpio_key), GFP_KERNEL);

	for (i = 0; i < count; i++) {

		gpio = of_get_gpio_flags(node, i, &flags);
		irq  = gpio_to_irq(gpio);

		gpio_keys[i].idx   = i;
		gpio_keys[i].gpio  = gpio;
		gpio_keys[i].gpiod = gpio_to_desc(gpio);
		gpio_keys[i].irq   = irq;
		gpio_keys[i].flag  = flags;

		if (flags & OF_GPIO_ACTIVE_LOW) {	// 如果在设备树节点中，设置了低电平有效；OF_GPIO_ACTIVE_LOW -> 这是 .c 文件中用到的状态值
			flags |= GPIOF_ACTIVE_LOW;		// GPIOF_ACTIVE_LOW -> 这是在 gpio 系统中用到的状态值
		}

  		err = devm_gpio_request_one(&pdev->dev, gpio, flags, NULL);
		if (err < 0) {
			
			dev_err(&pdev->dev, "Failed to request GPIO %d, error %d\n", gpio, err);
			return err;
		}

		printk("\tgpio %d irq %d\n", gpio, irq);

		// IRQF_TRIGGER_FALLING -> 下降沿触发，即按下
	    // 中断触发时，dev 会作为参数，传给 handler 函数
		err = request_irq(irq, reader_button_irq, IRQF_TRIGGER_FALLING, DRV_NAME, &gpio_keys[i]);
	}

	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);

	return 0;
}

static int button_irq_remove(struct platform_device *pdev)
{
	struct device_node *node = pdev->dev.of_node; 
	int count;
	int i;

	count = of_gpio_count(node);

	for (i = 0; i < count; i++) {

		free_irq(gpio_keys[i].irq, &gpio_keys[i]);
	}
	
	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);

	return 0;
}


const struct of_device_id button_dt_node_table[] = {
	{ .compatible = DT_COMPAT },
	{  }, 
};


static struct platform_driver button_irq_drv = {

	.probe = button_irq_probe,
	.remove = button_irq_remove,
	.driver = {
		.name = DRV_NAME,
		.of_match_table = button_dt_node_table,
	},
};

static int __init button_drv_init(void) {

	kque = keys_queue_create(MAXGKEYS);
	platform_driver_register(&button_irq_drv);
	printk(">>>>>>>>>> reader button platform_driver registered, ended! <<<<<<\n");
	return 0;
}

static void __exit button_drv_exit(void) {

	platform_driver_unregister(&button_irq_drv);
	printk(">>>>>>>>>> reader button platform_driver unregistered, ended! <<<<<<\n");
}

module_init(button_drv_init);
module_exit(button_drv_exit);

MODULE_LICENSE("GPL");

