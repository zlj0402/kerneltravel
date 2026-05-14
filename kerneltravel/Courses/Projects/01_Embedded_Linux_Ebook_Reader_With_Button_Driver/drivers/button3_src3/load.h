/**
 * @brief: 基于 <src3, button2>，将已经分离 gpio_irq_drv.ko 中的 platform_driver & file_operations & circle queue..., 构建出独立的模块;
 * @date: 8/10/2025
 *
 * @version-match: src3 <===> button3
 *		+ button 一直都是一个没有分离成功的最初尝试版本;
 *
 * @ps:
 * 	+ module_init/exit() 的函数是不能够，也不用 EXPORT_SYMBOL;
 * 		1. 因为别的模块没有调用;
 * 		2. 模块注册的时候就会被调用执行;
 *	+ 多个文件共用的工具结构，或者函数
 *		1. 用 .h 的头文件引入;
 *		2. 别的模块用的函数或者变量，也需要在 .c 文件中 EXPORT_SYMBOL，编译链接时才能被其他模块链接使用;
 *			例如，button_core.c
 *			+ 别的模块用到的变量:
 *				struct button_core g_button_core;
 *				EXPORT_SYMBOL(g_button_core);
 *			+ 别的模块用到的函数:
 *				// 导出供其他模块调用
 *				EXPORT_SYMBOL(button_core_init);
 *				EXPORT_SYMBOL(button_core_put_key);
 *				EXPORT_SYMBOL(button_core_get_key);
 *				EXPORT_SYMBOL(button_core_empty);
 *
 * @ps:
 * 	+ button2 -> 改名为 button2_src3
 * 	+ button3 -> 改名为 button3_src3
 *
 * 	+ button -> 没改名，一直都是错误分离的案例;
 *
 * @ps:
 * 	+ 设备树的设置，参照当时学习中断那一系列课程的 git:
 * 		+ link: https://github.com/zlj0402/kerneltravel/blob/main/kerneltravel/Courses/Embedded_Linux_application_development/05_linux_driver_development_basics/IRQ/01_gpio_irq_drv/load.h
 * 		+ 只有 1.3 两个引脚的设备树节点中的 compatible 由原来的 "zlj,gpio_key" -> 改为 "button_driver,gpio_key" 
 */
