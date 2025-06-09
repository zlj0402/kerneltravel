#include <linux/module.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/irqreturn.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/slab.h>

#include "load.h"

struct gpio_key {

	int gpio;
	int irq;
	enum of_gpio_flags flag;
};

static struct gpio_key* gpio_keys;

static irqreturn_t gpio_key_irq_zlj(int irq, void* dev_id) {

	struct gpio_key* gpio_key = dev_id;

	printk("key %d val %d\n", irq, gpio_get_value(gpio_key->gpio));

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

		gpio_keys[i].gpio = gpio;
		gpio_keys[i].irq = irq;
		gpio_keys[i].flag = flags;

		if (flags & OF_GPIO_ACTIVE_LOW) {	// 如果在设备树节点中，设置了低电平有效；OF_GPIO_ACTIVE_LOW -> 这是 .c 文件中用到的状态值
			flags |= GPIOF_ACTIVE_LOW;	// GPIOF_ACTIVE_LOW -> 这是在 gpio 系统中用到的状态值
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
		err = request_irq(irq, gpio_key_irq_zlj, IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING, "zlj_gpio_keys", &gpio_keys[i]);
	}

	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);

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

	return 0;
}


const struct of_device_id gpio_irq_table[] = {
	{ .compatible = "zlj,gpio_key" },
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
