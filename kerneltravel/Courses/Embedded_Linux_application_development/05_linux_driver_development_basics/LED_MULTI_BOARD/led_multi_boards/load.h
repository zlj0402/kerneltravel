/**
 * @brief: 把代码框架设计得，能够支持多个开发板;
 *
 * @step:
 * 		1. 在系统调用接口 open 调用到 file_operations.open(即led_open)的时候，
 * 			调用我们自己定义的struct led_operations结构的init，去ioremap映射寄存器地址，去设置GPIO5_3(LED对应的引脚)
 *		2. ... write 的时候，调用 file_operations(即led_write)的时候
 * 			调用我们自己定义的struct led_operations结构的ctl，去往LED对应的数据寄存器（GPIO5_DR）写值，控制LED的亮灭;
 * 		3. 当用户调用 close(fd)，或者程序结束，fd 被内核自动回收时，就会触发 led_release;
 * 			我们在其中，调用我们自己定义的struct led_operations结构的exit，去iounmap映射的虚拟地址;
 */
