#include <linux/kernel.h>
#include "led_opr.h"
#include "led_resource.h"

static struct led_resource* led_rsc;

static int board_demo_led_init(int which)
{
	if (!led_rsc)
		led_rsc = get_led_resource();

	printk("init gpio: group %d, pin %d\n", GROUP(led_rsc->pin), PIN(led_rsc->pin));
	switch(GROUP(led_rsc->pin)) 
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
	printk("set led %s: group %d, pin %d\n", status ? "on" : "off", GROUP(led_rsc->pin), PIN(led_rsc->pin));
	switch(GROUP(led_rsc->pin)) 
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

struct led_operations* get_board_led_opr(void)
{
	return &board_demo_led_opr;
}
