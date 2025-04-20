/**
 * @brief: 把代码框架设计得，能够支持多个开发板 -- 20/4/2025;
 *
 * @description:
 * 		1. 在系统调用接口 open 调用到 file_operations.open(即led_open)的时候，
 * 			调用我们自己定义的struct led_operations结构的init，去ioremap映射寄存器地址，去设置GPIO5_3(LED对应的引脚)
 *		2. ... write 的时候，调用 file_operations(即led_write)的时候
 * 			调用我们自己定义的struct led_operations结构的ctl，去往LED对应的数据寄存器（GPIO5_DR）写值，控制LED的亮灭;
 * 		3. 当用户调用 close(fd)，或者程序结束，fd 被内核自动回收时，就会触发 led_release;
 * 			我们在其中，调用我们自己定义的struct led_operations结构的exit，去iounmap映射的虚拟地址;
 * 		-------------------------------------------------------------------------------------------
 * 		4. board_A_led.c 完全是硬件资源（platform_device），当两者都注册进内核，匹配后，调用 platform_driver.probe，
 * 			+ 通过 platform_get_resource 得到硬件的资源，比如根据 pin 引脚的个数，来注册多少数量的 /dev/zlj_ledx，
 * 			+ 也同样在 platform_driver.probe 中，调动 led_drv.c 模块当中的函数;
 * 			+ ps:
 * 				以前第一份LED的程序，platform_driver 和 file_operations 是写在同一个模块的，
 * 				这里将对引脚的操作（platform_driver）和 驱动程序 file_operations 也分开了;
 * 				>>>>>巧妙的是通过 EXPORT_SYMBOL(func) 能够调动另一个模块（file_operations所在模块）执行函数；这样 platform_driver 和 platform_device 匹配后，才回去创建节点;<<<<<
 *
 * @steps:
 *		1. Makefile同路径下，执行：make
 * 		2. cp led_test led_drv.ko chip_demo_gpio.ko board_A_led.ko 到开发板和ubuntu共享的目录中
 * 		3. 进入到开发板shell
 * 		4. echo "7 4 1 7" > /proc/sys/kernel/printk，便于我们查看系统日志
 * 		5. 先insmod chip_demo_gpio.ko 于 insmod led_drv.ko 之前，因为 chip_demo_gpio 当中有几个函数，是定义在 led_drv.c 当中的;
 * 			不然会报错:
 * 				[root@100ask:/mnt/05_linux_drivers_development_basics/LED_MULTI_BOARD/03_led_multi_boards_platform_bus]# insmod chip_demo_gpio.ko
 * 				[   52.253211] chip_demo_gpio: Unknown symbol register_led_operations (err 0)
 * 				[   52.260888] chip_demo_gpio: Unknown symbol led_device_create (err 0)
 * 				[   52.269073] chip_demo_gpio: Unknown symbol led_device_destroy (err 0)
 * 				insmod: ERROR: could not insert module chip_demo_gpio.ko: Unknown symbol in module
 *		6. insmod board_A_led.c 不分先后，因为它没有依赖于其他模块的函数;
 * 		7. ./led_test /dev/zlj_led0 on/off
 * 
 * @output:
 * 		[root@100ask:/mnt/05_linux_drivers_development_basics/LED_MULTI_BOARD/03_led_multi_boards_platform_bus]# ./led_test /dev/zlj_led1 on
 * 		[  133.480086]
 * 		[	133.480086] /home/zlj/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/05_linux_driver_development_basics/LED_MULTI_BOARD/03_led_multi_boards_platform_bus/led_drv.c led_open 49, minor: 1
 * 		[  133.500833] init gpio: group 5, pin 8
 * 		[  133.505438]
 * 		[	133.505438] /home/zlj/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/05_linux_driver_development_basics/LED_MULTI_BOARD/03_led_multi_boards_platform_bus/led_drv.c led_write 71, minor: 1
 * 		[  133.526055] set led on: group 5, pin 8
 * 		[  133.530423]
 * 		[	133.530423] /home/zlj/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/05_linux_driver_development_basics/LED_MULTI_BOARD/03_led_multi_boards_platform_bus/led_drv.c led_release 81, minor: 1
 * 		[  133.551328]
 * 		[	133.551328] /home/zlj/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/05_linux_driver_development_basics/LED_MULTI_BOARD/03_led_multi_boards_platform_bus/chip_demo_gpio.c board_demo_led_exit line 43, led 1
 */
