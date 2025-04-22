#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/of.h>
#include "led_opr.h"
#include "led_resource.h"

static int g_ledpins[100];
static int g_ledcnt = 0;

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> led operations start <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
static int board_demo_led_init(int which)
{

	printk("init gpio: group %d, pin %d\n", GROUP(g_ledpins[which]), PIN(g_ledpins[which]));
	switch(GROUP(g_ledpins[which])) 
	{
		case 0: 
		{
			printk("init pin of group 0 ...\n");
			break;
		}
		case 1:
		{
			printk("init pin of group 1 ...\n");
			break;
		}
		case 2:
		{
			printk("init pin of group 2 ...\n");
			break;
		}
		case 3:
		{
			printk("init pin of group 3 ...\n");
			break;
		}
	}
	return 0;
}

static int board_demo_led_exit(int which) 
{
	printk("\n%s %s line %d, led %d\n", __FILE__, __FUNCTION__, __LINE__, which);
	return 0;
}

static int board_demo_led_ctl(int which, char status)
{
	printk("set led %s: group %d, pin %d\n", status ? "on" : "off", GROUP(g_ledpins[which]), PIN(g_ledpins[which]));
	switch(GROUP(g_ledpins[which])) 
	{
		case 0: 
		{
			printk("set pin of group 0 ...\n");
			break;
		}
		case 1:
		{
			printk("set pin of group 1 ...\n");
			break;
		}
		case 2:
		{
			printk("set pin of group 2 ...\n");
			break;
		}
		case 3:
		{
			printk("set pin of group 3 ...\n");
			break;
		}
	}
	return 0;
}

struct led_operations board_demo_led_opr = {
	.num = 1,
	.init = board_demo_led_init,
	.ctl = board_demo_led_ctl,
	.exit = board_demo_led_exit,
};
	
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> platform_driver <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

static int chip_demo_gpio_led_probe(struct platform_device* dev)
{
	int err;
	int led_pin;
	struct device_node* np;

	np = dev->dev.of_node;
	if (!np)
		return -1;
	
	err = of_property_read_u32(np, "pin", &led_pin);
	
	/* 记录引脚 */
	g_ledpins[g_ledcnt] = led_pin;
	/* device_create */
	led_device_create(g_ledcnt);
	g_ledcnt++;
	
	return 0;
}

static int chip_demo_gpio_led_remove(struct platform_device* dev)
{
	int i;
	int led_pin;
	int err;
	int del_cnt = 0;
	struct device_node* np;;
	
	np = dev->dev.of_node;
	if (!np)
		return -1;
	
	err = of_property_read_u32(np, "pin", &led_pin);

	for (i = 0; i < g_ledcnt; i++)
	{
		if (g_ledpins[i] == led_pin)
		{
			led_device_destroy(i);
			g_ledpins[i] = -1;
			del_cnt++;
			break;
		}
	}

	if (del_cnt == g_ledcnt)
		g_ledcnt = 0;

	if (!g_ledcnt)
		printk("zlj_led: all devices removed\n");
	
	return 0;
}

static const struct of_device_id zlj_led_match[] = {
	{ .compatible = "zlj,led", },
	{},
};


static struct platform_driver chip_demo_gpio_drv = {
	.probe = chip_demo_gpio_led_probe,
	.remove = chip_demo_gpio_led_remove,
	.driver = {
		.name = "zlj_led",
		.of_match_table = zlj_led_match,
	},
};

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> module <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

static int chip_demo_gpio_drv_init(void)
{
	int err;
	err = platform_driver_register(&chip_demo_gpio_drv);
	register_led_operations(&board_demo_led_opr);
	printk("%s %s chip_demo_gpio module loaded. \n", __FILE__, __FUNCTION__);
	return err;
}

static void chip_demo_gpio_drv_exit(void)
{
	platform_driver_unregister(&chip_demo_gpio_drv);
	printk("%s %s chip_demo_gpio module unloaded. \n", __FILE__, __FUNCTION__);
}

module_init(chip_demo_gpio_drv_init);
module_exit(chip_demo_gpio_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ZLJ");

