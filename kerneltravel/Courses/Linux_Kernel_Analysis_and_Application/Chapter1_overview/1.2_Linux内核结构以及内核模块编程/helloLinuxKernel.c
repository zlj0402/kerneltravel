#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

/*
 * 模块的初始化函数lkp_init()
 * __init是用于初始化的修饰符
 */
static int __init lkp_init(void)
{
	printk("<1>Hello, Linux kernel! from the kernel space...\n");
	return 0;
}

/*
 * 模块的退出和清理函数 lkp_exit()
 */
static void __exit lkp_exit(void)
{
	printk("<1>Goodbye, Linux kernel! leaving kernel space...\n");
}

module_init(lkp_init);
module_exit(lkp_exit);
/* 
 * 模块的许可证声明GPL
 */
MODULE_LICENSE("GPL");
