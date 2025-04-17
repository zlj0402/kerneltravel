#include <linux/kernel.h>
#include "led_opr.h"

static int board_demo_led_init(int which)
{
	printk("%s %s line %d, led %d\n", __FILE__, __FUNCTION__, __LINE__, which);
	return 0;
}

static int board_demo_led_ctl(int which, char status)
{
	printk("%s %s line %d, led %d, %d\n", __FILE__, __FUNCTION__, __LINE__, which, (int)status);
	return 0;
}

struct led_operations board_demo_led_opr = {
	.num = 1,
	.init = board_demo_led_init,
	.ctl = board_demo_led_ctl,
};

struct led_operations* get_board_led_opr(void)
{
	return &board_demo_led_opr;
}
