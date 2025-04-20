#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include "led_resource.h"

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> platform_device <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<s

static struct resource resources[] = {
	{
		.start = GROUP_PIN(3, 1),
		.flags = IORESOURCE_IRQ,
	},
	{
		.start = GROUP_PIN(5, 8),
		.flags = IORESOURCE_IRQ,
	},
}; 

void led_dev_release(struct device *dev)
{
	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
}


static struct platform_device board_A_led_dev = {
	.name = "zlj_led",
	.num_resources = ARRAY_SIZE(resources),
	.resource = resources,
	.dev = {
		.release = led_dev_release,
	},
};

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> module <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

static int led_dev_init(void)
{
	int err;
	err = platform_device_register(&board_A_led_dev);
	printk(">>>>> %s -- %s, board_A_led module loaded! <<<<<\n", __FILE__, __FUNCTION__);
	return 0;
}

static void led_dev_exit(void)
{
	platform_device_unregister(&board_A_led_dev);
	printk(">>>>> %s -- %s, board_A_led module unloaded! <<<<<\n", __FILE__, __FUNCTION__);
}

module_init(led_dev_init);
module_exit(led_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ZLJ");
