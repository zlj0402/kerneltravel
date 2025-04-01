/**
 * @brief: led driver of IMX6ULL board; -- 29/3/2025
 *
 * @steps:
 * 		1. driver的构造steps见git:
 * 			kerneltravel/Courses/Embedded_Linux_application_development/writing_APP_to_operate_hardware/first_drv/hello_drv
 * 		2. 这次是代码构建的设备节点: /dev/zljled
 * 			过程见hello_drv的第四个版本
 * 		3. 这次主要是，点亮LED的四个步骤:
 * 			3.1 设置CCM寄存器，使能对应的GPIO组;
 * 				+ CCM: Clock Control Module时钟控制单元;
 * 				+ IMX6ULL开发板上，LED对应是GPIO5，默认是开启的，我们无需设置; 
 * 				+ 但还是留下CCM对应寄存器的基地址:
 * 					+ CCM_CCGR1地址: 0x020C4000 + 0×6C
 * 			3.2 设置IOMUXC的寄存器，选择引脚的复用模式为GPIO;
 * 				+ IOMUXC: I/O Multiplexer Controller，输入输出多路复用寄存器;
 * 				+ IMX6ULL开发板上，设置寄存器: IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3
 * 					+ 基地址: IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3地址：0x2290000 + 0x14
 * 					+ 多路复用模式选择MUX_MODE: 该寄存器的低4位;
 * 						+ 在注册字符驱动程序后，映射该寄存器的物理地址，得到虚拟地址:
 * 							+ 因为，我们不能直接对寄存器的物理地址操作，*p = 5; 这种赋值，都是对虚拟地址进行操作; 所以我们需要对物理地址进行映射ioremap()
 * 							+ static volatile unsigned long* IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3;
 * 							+ IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 = ioremap(0x2290000 + 0x14, sizeof(unsigned long));
 * 							+ // config pins as gpio mode
 * 							+ *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 &= ~0xf;
 * 							+ *IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 |= 0x5;
 * 						+ 该寄存器的MUX_MODE后4位，写入5，就表示引脚工作为GPIO模式;
 * 			3.3 设置GPIO5组的GPIO5_GDIR方向寄存器，引脚的对应位: GPIO5_3
 * 				+ 把GPIO5的GDIR第3位设置为1; GPIO5_3引脚就成为输出引脚;
 * 				+ IMX6ULL开发板上，设置寄存器: GPIO5_GDIR
 * 					+ GPIO5_GDIR地址：0x020AC004
 * 					+ static volatile unsigned long* GPIO5_GDIR;
 * 					+ GPIO5_GDIR = ioremap(0x020AC004, sizeof(unsigned long));
 * 					+ // 程序中，我们采用在led_open的时候，在configure as gpio mode之后，设置引脚为output模式;
 * 					+ *GPIO5_GDIR |= (1 << 3);	// 把GPIO5的方向寄存器的第3位设置为1;
 * 				+ 这样就设置好引脚pin为output模式;
 * 			3.4 设置GPIO5组的GPIO5_DR数据寄存器，参考引脚对应位: GPIO5_3
 * 				+ 把GPIO5的DR第3位设置为1/0; GPIO5_3引脚状态就成为高/低电平;
 * 				+ IMX6ULL开发板上，设置寄存器GPIO5_DR
 * 					+ GPIO5_DR地址：0x020AC000
 * 					+ static volatile unsigned long* GPIO5_DR;
 * 					+ GPIO5_DR = ioremap(0x020AC000, sizeof(unsigned long));
 * 					+ // 确保在引脚为output模式后; 当调用./led_test /dev/zljled on时，写入1/0值（led_write()）;
 * 					+ *GPIO5_DR &= ~(1 << 3);	// 使引脚为低电平0
 * 					+ *GPIO5_DR |= (1 << 3);	// 使引脚为高电平
 * 
 * @execute & steps:
 * 		1. 在ubuntu源文件目录下，执行：make
 * 		2. cp led_drv.ko 和 led_test 到ubuntu和开发板共享目录下;
 * 		3. 插入模块：insmod led_drv.ko
 * 		4. 执行测试程序：./led_test /dev/zljled on/off
 * 		5. 开发板上对应的等就亮/灭;
 *
 * @ps:
 * 		printk("led_release: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
 * 		这句打印，不适合封装成一个函数调用，不然打印出来的__FUNCTION__和__LINE__都是封装函数的位置;
 * 		跟你原来的意愿（想去知道哪个函数进行的该printk）就违背了;
 * 		想使用这句，就直接将printk写入该函数;
 */
