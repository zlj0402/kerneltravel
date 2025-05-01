/**
 * @brief: GPIO: LED driver; LED glitters by waved-like time interval; -- platfrom_driver && platform_device(dtbs) -- 3/4/2025
 *
 * @steps:
 *      1. 驱动的 总线-设备-驱动 分离模型进行，可以完全参考git:
 *      	kerneltravel/Courses/Embedded_Linux_application_development/writing_APP_to_operate_hardware/first_drv/hello_drv_v3_dtbs
 *      	+ platform_device是以设备树的方式进行的;
 *      	+ platform_driver.driver中添加.of_match_table（of_device_id数组），来匹配设备树中的platform_device对应节点;
 *      	+ 然后在platform_driver.probe当中，以API of_property_read_string_index 得到platform_device设备节点的属性值;
 *      2. 驱动程序的注册，也同样是在platform_driver和platform_device相匹配之后进行注册;
 *      	同样，可以参考上面的git，有在上面使用到;
 *      3. 这次，用到了在代码中，创建设备节点/dev/<dev_name>，和设备目录/sys/class/<class_name>/<dev_name>
 *      	以前也有描述过步骤，见git的load.h:
 *      	kerneltravel/Courses/Embedded_Linux_application_development/writing_APP_to_operate_hardware/first_drv/hello_drv_v4_dtbs_create_dev_node
 *      	这个git有做过对于设备节点的创建过程;
 * 		4. 这次主要是，点亮LED的四个步骤:
 * 			4.1 设置CCM寄存器，使能对应的GPIO组;
 * 				+ CCM: Clock Control Module时钟控制单元;
 * 				+ IMX6ULL开发板上，LED对应是GPIO5，默认是开启的，我们无需设置; 
 * 				+ 但还是留下CCM对应寄存器的基地址:
 * 					+ CCM_CCGR1地址: 0x020C4000 + 0×6C
 * 			4.2 设置IOMUXC的寄存器，选择引脚的复用模式为GPIO;
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
 * 			4.3 设置GPIO5组的GPIO5_GDIR方向寄存器，引脚的对应位: GPIO5_3
 * 				+ 把GPIO5的GDIR第3位设置为1; GPIO5_3引脚就成为输出引脚;
 * 				+ IMX6ULL开发板上，设置寄存器: GPIO5_GDIR
 * 					+ GPIO5_GDIR地址：0x020AC004
 * 					+ static volatile unsigned long* GPIO5_GDIR;
 * 					+ GPIO5_GDIR = ioremap(0x020AC004, sizeof(unsigned long));
 * 					+ // 程序中，我们采用在led_open的时候，在configure as gpio mode之后，设置引脚为output模式;
 * 					+ *GPIO5_GDIR |= (1 << 3);	// 把GPIO5的方向寄存器的第3位设置为1;
 * 				+ 这样就设置好引脚pin为output模式;
 * 			4.4 设置GPIO5组的GPIO5_DR数据寄存器，参考引脚对应位: GPIO5_3
 * 				+ 把GPIO5的DR第3位设置为1/0; GPIO5_3引脚状态就成为高/低电平;
 * 				+ IMX6ULL开发板上，设置寄存器GPIO5_DR
 * 					+ GPIO5_DR地址：0x020AC000
 * 					+ static volatile unsigned long* GPIO5_DR;
 * 					+ GPIO5_DR = ioremap(0x020AC000, sizeof(unsigned long));
 * 					+ // 确保在引脚为output模式后; 当调用./led_test /dev/zljled on时，写入1/0值（led_write()）;
 * 					+ *GPIO5_DR &= ~(1 << 3);	// 使引脚为低电平0
 * 					+ *GPIO5_DR |= (1 << 3);	// 使引脚为高电平
 *		---------------------------------------------------------
 *		5. 实现：LED的闪烁，间隔时间由短变长，再由长变短; -- 用到了高精度的定时器struct hrtimer:
 *				背景：./led_test /dev/zljled on 的执行，让LED如上那么闪烁;
 *				5.1 声明一个全局struct hrtimer timer变量;
 *					static struct hrtimer timer;
 *				5.2 初始化timer;
 *					// void hrtimer_init(struct hrtimer *timer, clockid_t clock_id, enum hrtimer_mode mode)
 *					hrtimer_init(&timer, CLOCK_MONOTONIC, HRTIMER_MODE_PINNED);
 *						+ CLOCK_MONOTONIC（推荐）：单调递增的时间（不会因系统时间调整而回退）;
 *						+ HRTIMER_MODE_PINNED: 相对时间触发，固定CPU，定时器不会迁移到其他CPU上;
 *				5.3 指定定时任务，即回调函数;
 *					timer.function = monitor_handler;
 *					...
 *					static enum hrtimer_restart monitor_handler(struct hrtimer *hrtimer)
 *						+ monitor_handler: 
 *							+ content: >>>>> 在该回调函数当中，进行LED灯亮灭和时间间隙的控制; <<<<<
 *							+ return: enum hrtimer_restart类型，决定了timer是:
 *								+ HRTIMER_NORESTART,  // 只触发一次（单次定时器）
 *								+ HRTIMER_RESTART     // 继续运行（周期性定时器）
 *				5.3 开启一个定时任务;
 *					// hrtimer_start_range_ns(struct hrtimer *timer, ktime_t tim, u64 delta_ns, const enum hrtimer_mode mode);
 *					hrtimer_start_range_ns(&timer, TIMES_INTERVAL(2), 0, HRTIMER_MODE_REL_PINNED);
 *						+ ktime_t tim: 定时器的超时时间;
 *						+ u64 delta_ns: 定时器可接受的最大延迟，单位是纳秒 (ns)。设为 0 表示无延迟，即精确触发。
 *						+ HRTIMER_MODE_REL_PINNED: 定时器模式，相对当前时间启动，并固定到当前 CPU，即该定时器只会在当前 CPU 上触发，不会迁移到其他 CPU。
 *				5.4 取消一个定时任务;
 *					hrtimer_cancel(&timer); 主要作用:
 *						1) 停止定时器，防止它再次触发回调函数
 *						2) 等待回调函数执行完毕（如果定时器已触发但回调函数仍在运行）
 *						3) 返回值表明定时器是否成功取消（1 表示定时器在运行并被取消，0 表示定时器本来就没有在运行）
 *				------------
 *				ps: 
 *					1. 需要考虑很多情况:
 *						a. 多次执行测试程序./led_test /dev/zljled on，多次触发start_timer()? 
 *							需要判断timer是否在就绪队列当中: if (hrtimer_is_queued(&timer))
 *						b. 第一次start_timer时，我们加的判断，timer是否激活？避免重复start_timer; 
 *						c. 没init的timer，被启动，会报错: Unable to handle kernel NULL pointer dereference at virtual address 00000000
 *					2. 检查是否已经就绪等待触发（即在工作中）: if (hrtimer_is_queued(&timer)) 
 *					3. timer有无初始化判断: if (!timer.function) -> 未初始化
 *					4. LED每次都是执行一次亮灭，再控制间隙时长;
 *						一开始，写成每次只要么亮一下，或者灭一下; 虽然有一会亮一会灭，但感受不到忽快忽慢;
 * 
 * @execute & steps:
 * 		1. 设备树的修改:
 * 			1.1 在/Linux-4.9.88/arch/arm/boot/dts$ vim 100ask_imx6ull-14x14.dts当中添加节点;
 * 			1.2 在linux kernel根目录，执行make dtbs
 * 			1.3 将生成的dtb的二进制文件 arch/arm/boot/dts/100ask_imx6ull-14x14.dtb 到ubuntu和开发板共享的目录下;
 * 			1.4 开发板中，将dtb文件cp到/boot目录下，覆盖原来的dtb；然后重启
 * 			1.5 查看有没有生成设备树节点
 * 				cd /sys/firmware/devicetree/base/(设备树的根目录)，看看有没有创建的新节点led_zlj
 * 			1.6 查看有没有生成platform_device
 * 				cd /sys/bus/platform/devices
 * 		2. 驱动程序模块的注册;
 * 			2.1 在ubuntu源文件目录下，执行：make
 * 			2.2 cp led_drv.ko 和 led_test 到ubuntu和开发板共享目录下;
 * 			2.3 echo "7 4 1 7" > /proc/sys/kernel/printk，这样能查看我们加的printk;
 * 			2.4 插入模块：
 * 				insmod led_drv.ko
 * 			2.5 执行测试程序：./led_test /dev/zljled on/off
 * 			2.6 现象:
 * 				+ on -> 灯忽快忽慢
 * 		 		+ off -> 灯灭
 *
 * @ps:
 * 		printk("led_release: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
 * 		这句打印，不适合封装成一个函数调用，不然打印出来的__FUNCTION__和__LINE__都是封装函数的位置;
 * 		跟你原来的意愿（想去知道哪个函数进行的该printk）就违背了;
 * 		想使用这句，就直接将printk写入该函数;
 */
