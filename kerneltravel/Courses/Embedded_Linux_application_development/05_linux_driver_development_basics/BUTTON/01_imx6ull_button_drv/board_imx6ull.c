#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include "button_drv.h"


/* 每个GPIO组有8个寄存器，每个寄存器4字节 */
struct imx6ull_gpio {
    volatile unsigned int dr;
    volatile unsigned int gdir;
    volatile unsigned int psr;
    volatile unsigned int icr1;
    volatile unsigned int icr2;
    volatile unsigned int imr;
    volatile unsigned int isr;
    volatile unsigned int edge_sel;
};

/* enable GPIO4, reg addr: 0x20c4074 */
static volatile unsigned long *CCM_CCGR3; 

/* enable GPIO5, reg addr: 0x20c406c */
static volatile unsigned long *CCM_CCGR1; 

/* set GPIO5_IO03 as GPIO */
static volatile unsigned long *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER1;

/* set GPIO4_IO14 as GPIO */
static volatile unsigned long *IOMUXC_SW_MUX_CTL_PAD_NAND_CE1_B;

/* GPIO4 的寄存器组起始地址：0x20A8000*/
static struct imx6ull_gpio *gpio4;
/* GPIO5 的寄存器组起始地址：0x20AC000 */
static struct imx6ull_gpio *gpio5;


static void board_imx6ull_button_init_gpio(int which)
{
	printk("%s %s %d, init gpio for button %d\n", __FILE__, __FUNCTION__, __LINE__, which);

	if (!CCM_CCGR1)
	{
		CCM_CCGR1 = ioremap(0x20c406c, PAGE_SIZE);
		CCM_CCGR3 = ioremap(0x20c4074, PAGE_SIZE);

		IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER1 = ioremap(0x229000C, PAGE_SIZE);
		IOMUXC_SW_MUX_CTL_PAD_NAND_CE1_B = ioremap(0x20E01B0, PAGE_SIZE);

		// iomux = ioremap();
		gpio4 = ioremap(0x20A8000, PAGE_SIZE);
		gpio5 = ioremap(0x20AC000, PAGE_SIZE);
	}

	if (which == 0)
	{
		/* 1. enable GPIO5
		 * CG15, b[31:30] = 0b11
		 */
		*CCM_CCGR1 |= (0b11 << 30);

		 /* 2. set GPIO5_IO01 as GPIO
		  * MUX_MODE, b[3:0] = 0b101
		  */
		 *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER1 &= ~(0xf); 
		 *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER1 |= 0b0101;

		 /* 3. set GPIO5_IO01 as input
		  * GPIO5 GDIR, b[1] = 0b0
		  */
		 // gpio5 组 idx 为 1 个引脚状态
		 gpio5->gdir &= ~(1 << 1);
	}
	else if (which == 1) 
	{
		/* 1. enable GPIO4
		 * CG6, b[13:12] = 0b11
		 */
		*CCM_CCGR3 |= (0b11 << 12);

		/* 2. set GPIO4_IO14 as GPIO
		 * MUX_MODE, b[3:0] = 0b101
		 */
		*IOMUXC_SW_MUX_CTL_PAD_NAND_CE1_B &= ~(0Xf);
		*IOMUXC_SW_MUX_CTL_PAD_NAND_CE1_B |= 0b0101;

		/* 3. set GPIO4_IO14 as input
		 * GPIO4 GDIR, b[14] = 0b0
		 */
		// gpio4 组 idx 为 14 个引脚状态
		gpio4->gdir &= ~(1 << 14);
	}
}

static int board_imx6ull_button_read_gpio(int which)
{
	printk("%s %s %d, init gpio for button %d\n", __FILE__, __FUNCTION__, __LINE__, which);
	if (which == 0)
		return (gpio5->psr & (1 << 1)) ? 1 : 0;
	else
		return (gpio4->psr & (1 << 14)) ? 1 : 0;
}

static struct button_operations my_buttons_ops = {
	.count = 2,
	.init = board_imx6ull_button_init_gpio,
	.read = board_imx6ull_button_read_gpio,
};

static int __init board_imx6ull_button_init(void)
{
	register_button_operations(&my_buttons_ops);
	printk(">>>>> %s -- %s, board_imx6ull module loaded! <<<<<\n", __FILE__, __FUNCTION__);
	return 0;
}

static void __exit board_imx6ull_button_exit(void)
{
	unregister_button_operations();
	printk(">>>>> %s -- %s, board_imx6ull module unloaded! <<<<<\n", __FILE__, __FUNCTION__);
}

module_init(board_imx6ull_button_init);
module_exit(board_imx6ull_button_exit);

MODULE_LICENSE("GPL");
