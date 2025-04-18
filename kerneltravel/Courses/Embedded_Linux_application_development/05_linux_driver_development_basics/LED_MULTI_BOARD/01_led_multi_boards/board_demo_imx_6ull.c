#include <linux/kernel.h>
#include <asm/io.h>
#include "led_opr.h"

/* registers */
// IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3地址：0x2290000 + 0x14
static volatile unsigned long* IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3;

// GPIO5_GDIR地址：0x020AC004
static volatile unsigned long* GPIO5_GDIR;

// GPIO5_DR地址：0x020AC000
static volatile unsigned long* GPIO5_DR;


static int board_demo_led_init(int which)
{
	if (which == 0)
	{
		if (!GPIO5_GDIR)
		{
			IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 = ioremap(0x2290000 + 0x14, PAGE_SIZE);
			GPIO5_GDIR = ioremap(0x020AC004, PAGE_SIZE);
			GPIO5_DR = ioremap(0x020AC000, PAGE_SIZE);
		}

		/* enable GPIO5 -- default enabled */
		
		/* config pins as gpio mode*/
		*IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 &= ~0xf;
		*IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 |= 0x5;

		/* config gpio as output */
		*GPIO5_GDIR |= (1 << 3);
	
	}
	return 0;
}

static int board_demo_led_exit(int which) 
{
	if (which)
	{
		iounmap(GPIO5_DR);
		iounmap(GPIO5_GDIR);
		iounmap(IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3);
	}
	return 0;
}

static int board_demo_led_ctl(int which, char status)
{
	if (which == 0)
	{
		if (!status)
			// 输出高电平, 灯灭
			*GPIO5_DR |= (1 << 3);
		else
			// 输出低电平，灯亮
			*GPIO5_DR &= ~(1 << 3);
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
