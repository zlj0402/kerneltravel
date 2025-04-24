#include <linux/module.h>
#include <linux/kernel.h>
#include "button_drv.h"

static void board_xxx_button_init_gpio(int which)
{
	printk("%s %s %d, init gpio for button %d\n", __FILE__, __FUNCTION__, __LINE__, which);
}

static int board_xxx_button_read_gpio(int which)
{
	printk("%s %s %d, init gpio for button %d\n", __FILE__, __FUNCTION__, __LINE__, which);
	return 1;
}

static struct button_operations my_buttons_ops = {
	.count = 2,
	.init = board_xxx_button_init_gpio,
	.read = board_xxx_button_read_gpio,
};

static int __init board_xxx_button_init(void)
{
	register_button_operations(&my_buttons_ops);
	printk(">>>>> %s -- %s, board_xxx module loaded! <<<<<\n", __FILE__, __FUNCTION__);
	return 0;
}

static void __exit board_xxx_button_exit(void)
{
	unregister_button_operations();
	printk(">>>>> %s -- %s, board_xxx module unloaded! <<<<<\n", __FILE__, __FUNCTION__);
}

module_init(board_xxx_button_init);
module_exit(board_xxx_button_exit);

MODULE_LICENSE("GPL");
