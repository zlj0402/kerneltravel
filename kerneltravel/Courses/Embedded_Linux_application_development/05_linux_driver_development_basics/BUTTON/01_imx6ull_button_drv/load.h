/*
 * @brief: button drv -- imx6ull
 * 
 * @description:
 * 		+ 没有用 platform_device 和 platform_driver
 * 		+ 只是简单的注册了个 file_operations 驱动程序
 * 		---------------------------------------------
 * 		+ 对应具体单板的寄存器操作，使能，设置，读取
 * 		+ 1. 设置 CCM_CCGR1[31:30] 和 CCM_CCGR3[13:12] 分别使能 GPIO5 和 GPIO4
 * 		+ 2. 设置对应 IOMUXC 寄存器的后四位为 0b0101 (引脚为GPIO模式)
 * 		+ 3. 设置 gpio->gdir 对应引脚位为 1 (输入引脚)
 * 			+ GPIO5 的 idx 1 引脚 -> GPIO5_01
 * 			+ GPIO5 的 idx 14 引脚 -> GPIO4_14
 * 		+ 4. 读取 gpio->psr 对应引脚位的状态;
 * 			+ GPIO5_01 -> return (gpio5->psr & (1 << 1)) ? 1 : 0;
 * 			+ GPIO4_14 -> return (gpio4->psr & (1 << 14)) ? 1 : 0;
 * 		+ >>>>>>>>>>>> 这次练习，新颖的地方是，为 gpio 组以结构体的方式，映射了整个 gpio 组的寄存器; <<<<<<<<<<<<<<
 *
 * @steps:
 * 		+ 执行: make
 * 		+ 将 board_xxx.ko button_drv.ko button_test 一起，拷贝到 ubuntu 和开发板共享的目录;
 * 		+ 先 insmod button_drv.ko，后 insmod board_xxx.ko;
 * 			因为有函数依赖
 * 		+ 执行：./button_test /dev/zlj_button0
 */