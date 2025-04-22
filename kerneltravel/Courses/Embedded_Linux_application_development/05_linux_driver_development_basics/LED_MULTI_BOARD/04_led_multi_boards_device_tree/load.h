/**
 * @brief: 把代码框架设计得，能够支持多个开发板; 多个platform_device对应一个platform_driver -- 22/4/2025;
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
 * 		--------------------------------------------------
 * 		5. 现在以 设备树 的方式，进行 platform_device 的注册;
 * 			+ 原来:
 * 				+ 是在 platform_device 所在的驱动模块 board_A_led.c 中，是通过定义的 static struct resource 对象，放到 platform_device.resource 当中;
 *				+ 在与 platform_driver 匹配后，platfomr_driver.probe 被调用执行，原来是通过 platform_get_resource 接口得到资源，然后根据资源数，循环进行设备节点的创建;
 *			+ 现在:
 *				+ 在设备树的根节点下面，创建设备节点:
 *					#define GROUP_PIN(g,p) ((g<<16)|(p))
 *					/{
 *						zljled@0 {
 *							compatible = "zlj,led";
 *							pin = <GROUP_PIN(3, 1)>;
 *						}
 *
 *						zljled@1 {
 *							compatible = "zlj,led";
 *							pin = <GROUP_PIN(5, 8)>;
 *						}
 *					}
 *				+ 系统起来的时候，会由内核将两个设备节点转换为两个 platform_device;
 *				+ platform_driver 由模块被注册进内核是，两个 platform_device 都会与 platform_driver 相匹配;
 *					不用自己手动在 platform.probe 函数中，通过循环来注册;
 *				+ 注册的资源节点数不会少，我们在记录下资源后，还是需要在 platform_driver 所在驱动模块被 rmmod 时，
 *					通过在 platform_driver.remove 函数，循环进行所创建的设备节点destroy;
 *					
 *
 * @steps:
 * 	+ 设备树的修改:
 * 		1. vim Linux-4.9.88/arch/arm/boot/dts/100ask_imx6ull-14x14.dts
 * 		2. 添加设备节点:
 * 			#define GROUP_PIN(g,p) ((g << 16) | (p))
 * 			/ {
 * 				model = "Freescale i.MX6 ULL 14x14 EVK Board";
 * 				compatible = "fsl,imx6ull-14x14-evk", "fsl,imx6ull";
 * 				...
 * 				 zljled@0 {
 * 				 	compatible = "zlj,led";
 * 				 	pin = <GROUP_PIN(3, 1)>;
 * 				 };
 *
 * 				 zljled@1 {
 * 				 	compatible = "zlj,led";
 * 				 	pin = <GROUP_PIN(5, 8)>;
 * 				 };
 * 				...
 * 		3. 内核目录下，执行: Linux-4.9.88$ make dtbs
 * 		4. 将 Linux-4.9.88/arch/arm/boot/dts/100ask_imx6ull-14x14.dtb 文件拷贝到 ubuntu 和 开发板共享目录;
 * 		5. 进入开发板shell
 * 		6. 将 dtb 文件拷贝到 /boot 下，覆盖原来的文件;
 * 		7. reboot
 * 		8. 就能在对应的目录下看到 platform_device;
 *
 * 	+ 驱动模块:
 *		1. Makefile同路径下，执行：make
 * 		2. cp led_test led_drv.ko chip_demo_gpio.ko 到开发板和ubuntu共享的目录中
 * 		3. 进入到开发板shell
 * 		4. echo "7 4 1 7" > /proc/sys/kernel/printk，便于我们查看系统日志
 * 		5. 先insmod chip_demo_gpio.ko 于 insmod led_drv.ko 之前，因为 chip_demo_gpio 当中有几个函数，是定义在 led_drv.c 当中的;
 * 			不然会报错:
 * 				[root@100ask:/mnt/05_linux_drivers_development_basics/LED_MULTI_BOARD/03_led_multi_boards_platform_bus]# insmod chip_demo_gpio.ko
 * 				[   52.253211] chip_demo_gpio: Unknown symbol register_led_operations (err 0)
 * 				[   52.260888] chip_demo_gpio: Unknown symbol led_device_create (err 0)
 * 				[   52.269073] chip_demo_gpio: Unknown symbol led_device_destroy (err 0)
 * 				insmod: ERROR: could not insert module chip_demo_gpio.ko: Unknown symbol in module
 * 		6. ./led_test /dev/zlj_led0 on/off
 * 
 * @output:
[root@100ask:/mnt/05_linux_drivers_development_basics/LED_MULTI_BOARD/04_led_multi_boards_device_tree]# lsmod
Module                  Size  Used by
8723bu                918831  0
inv_mpu6050_spi         2320  0
inv_mpu6050            11894  2 inv_mpu6050_spi
evbug                   2282  0
100ask_adxl345_spi      4385  0
100ask_spidev           9883  0
100ask_irda             3736  0
100ask_rc_nec           1248  0
100ask_dht11            4242  0
100ask_ds18b20          4532  0
[root@100ask:/mnt/05_linux_drivers_development_basics/LED_MULTI_BOARD/04_led_multi_boards_device_tree]# insmod chip_demo_gpio.ko
[  104.665492] chip_demo_gpio: Unknown symbol register_led_operations (err 0)
[  104.672499] chip_demo_gpio: Unknown symbol led_device_create (err 0)
[  104.680711] chip_demo_gpio: Unknown symbol led_device_destroy (err 0)
insmod: ERROR: could not insert module chip_demo_gpio.ko: Unknown symbol in module
[root@100ask:/mnt/05_linux_drivers_development_basics/LED_MULTI_BOARD/04_led_multi_boards_device_tree]# insmod led_drv.ko
[  114.297963] led_class is created.
[  114.301378] >>>>> /home/zlj/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/05_linux_driver_development_basics/LED_MULTI_BOARD/04_led_multi_boards_device_tree/led_drv.c -- led_init, led_drv module loaded! <<<<<
[root@100ask:/mnt/05_linux_drivers_development_basics/LED_MULTI_BOARD/04_led_multi_boards_device_tree]# ins[  116.956201] RTL871X: RTW_ADAPTIVITY_EN_[  116.959898] AUTO, chplan:0x20, Regulation:3,3
[  116.964404] RTL871X: RTW_ADAPTIVITY_MODE_[  116.968251] NORMAL
mod [  119.574660] RTL871X: nolinked power save leave
io.ko emo_gpi
[  121.244983] /home/zlj/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/05_linux_driver_development_basics/LED_MULTI_BOARD/04_led_multi_boards_device_tree/chip_demo_gpio.c chip_demo_gpio_drv_init chip_demo_gpio module loaded.
[root@100ask:/mnt/05_linux_drivers_development_basics/LED_MULTI_BOARD/04_led_multi_boards_device_tree]# [  121.432201] RTL871X: nolinked power save enter

[root@100ask:/mnt/05_linux_drivers_development_basics/LED_MULTI_BOARD/04_led_multi_boards_device_tree]# ./led_test /dev/zlj_led0 on
[  141.612461]
[  141.612461] /home/zlj/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/05_linux_driver_development_basics/LED_MULTI_BOARD/04_led_multi_boards_device_tree/led_drv.c led_open 49, minor: 0
[  141.633066] init gpio: group 3, pin 1
[  141.638149] init pin of group 3 ...
[  141.641744]
[  141.641744] /home/zlj/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/05_linux_driver_development_basics/LED_MULTI_BOARD/04_led_multi_boards_device_tree/led_drv.c led_write 71, minor: 0
[  141.662807] set led on: group 3, pin 1
[  141.667225] set pin of group 3 ...
[  141.671287]
[  141.671287] /home/zlj/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/05_linux_driver_development_basics/LED_MULTI_BOARD/04_led_multi_boards_device_tree/led_drv.c led_release 81, minor: 0
[  141.692133]
[  141.692133] /home/zlj/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/05_linux_driver_development_basics/LED_MULTI_BOARD/04_led_multi_boards_device_tree/chip_demo_gpio.c board_demo_led_exit line 45, led 0
[root@100ask:/mnt/05_linux_drivers_development_basics/LED_MULTI_BOARD/04_led_multi_boards_device_tree]# ./led_test /dev/zlj_led0 off
[  147.195917]
[  147.195917] /home/zlj/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/05_linux_driver_development_basics/LED_MULTI_BOARD/04_led_multi_boards_device_tree/led_drv.c led_open 49, minor: 0
[  147.216819] init gpio: group 3, pin 1
[  147.220627] init pin of group 3 ...
[  147.224270]
[  147.224270] /home/zlj/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/05_linux_driver_development_basics/LED_MULTI_BOARD/04_led_multi_boards_device_tree/led_drv.c led_write 71, minor: 0
[  147.247953] set led off: group 3, pin 1
[  147.251802] set pin of group 3 ...
[  147.256351]
[  147.256351] /home/zlj/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/05_linux_driver_development_basics/LED_MULTI_BOARD/04_led_multi_boards_device_tree/led_drv.c led_release 81, minor: 0
[  147.276897]
[  147.276897] /home/zlj/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/05_linux_driver_development_basics/LED_MULTI_BOARD/04_led_multi_boards_device_tree/chip_demo_gpio.c board_demo_led_exit line 45, led 0
*/
