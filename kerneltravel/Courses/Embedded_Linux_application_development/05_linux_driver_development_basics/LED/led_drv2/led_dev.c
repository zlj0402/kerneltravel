#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/module.h>

#define LED_DEVICE_NAME "zlj_led_drv_"


static struct resource led_resources[] = {
	{
		.name  = "led_device123",	// 资源的名称
		.start = 0,					// 资源的起始地址
		.end   = 3,					// 资源的结束地址
		.flags = IORESOURCE_MEM,	// 资源的标志
	}
};

void led_dev_release(struct device *dev)
{
	printk("%s\n", __FUNCTION__);
}


static struct platform_device led_device = {
	.name = LED_DEVICE_NAME,
	.dev = {
		.release = led_dev_release,
	},
	.id = -1,
	.num_resources = ARRAY_SIZE(led_resources),
	.resource = led_resources,
};

static int __init led_dev_init(void)
{
	int err;
	err = platform_device_register(&led_device);

	printk(">>>>> %s -- %s, led_dev module loaded! <<<<<\n", __FILE__, __FUNCTION__);
	return err;
}

static void __exit led_dev_exit(void)
{
	platform_device_unregister(&led_device);
	printk(">>>>> %s -- %s, led_dev module unloaded! <<<<<\n", __FILE__, __FUNCTION__);
}

module_init(led_dev_init);
module_exit(led_dev_exit);

MODULE_LICENSE("GPL");
