/**
 * @brief: GPIO: LED driver; LED glitters by waved-like time interval; -- platfrom_driver && platform_device(dtbs) -- only one led -- 9/5/2025
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
 *
 * 			4.2 设置IOMUXC的寄存器，选择引脚的复用模式为GPIO;
 * 				+ IOMUXC: I/O Multiplexer Controller，输入输出多路复用寄存器;
 * 				+ IMX6ULL开发板上，设置寄存器: IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3
 * 					+ 是由 GPIO 子系统与 pinctrl 子系统，在设备树当中就已经完成了该内容;
 * 						+ / {
 *
 * 							...
 * 							
 * 					        zlj_led_ {
 * 					                compatible = "zlj_led_drv";
 * 					                version = "version1";
 * 					                pinctrl-names = "default";
 * 					                princtrl-0 = <&zlj_led_for_gpio_subsys>;
 * 					                led-gpios = <&gpio5 3 GPIO_ACTIVE_LOW>;
 * 					        };
 * 					        
 * 					        ...
 * 					        
 * 					        &iomuxc_snvs {
 * 					        pinctrl-names = "default_snvs";
 * 					        pinctrl-0 = <&pinctrl_hog_2>;
 * 					        
 * 					        zlj_led_for_gpio_subsys: zlj_led_for_gpio_subsys {        // < Function assigned for the core: Cortex-A7[ca7]
 * 					                fsl,pins = <
 * 					                MX6ULL_PAD_SNVS_TAMPER3__GPIO5_IO03        0x000110A0
 * 					            >;
 * 					        };
 * 			4.3 驱动代码中获取 gpio5_3 引脚的对象;
 * 				+ static struct gpio_desc* led_gpio;
 * 				+ led_gpio = gpiod_get(&pdev->dev, "led", 0);
 * 					+ 去设备树里找到对应 GPIO 属性：led-gpios = <...>;;
 * 
 * 			4.4 设置 GPIO5 的 GPIO5_DIR 方向寄存器的第3位，来决定 GPIO5_3 为输出/输入引脚;
 * 				+ gpiod_direction_output(led_gpio, 0);
 * 
 * 			4.5 设置 GPIO5 组的 GPIO5_DR 数据寄存器，参考引脚对应位: GPIO5_3
 * 				+ 把 GPIO5 的 DR 第 3 位设置为 1/0; GPIO5_3 引脚状态就成为高/低电平;
 * 					+ gpiod_set_value(led_gpio, 0);
 * 						+ 0 是逻辑值，不亮;
 *		---------------------------------------------------------
 *		5. 实现：LED的闪烁，间隔时间由短变长，再由长变短; -- 用到了高精度的定时器struct hrtimer:
 *				背景：./led_test /dev/zljled on 的执行，让LED如上那么闪烁;
 *				5.1 声明一个全局 struct hrtimer on_timer, off_timer 变量;
 *					static struct hrtimer on_timer;
 * 					static struct hrtimer off_timer;
 *				5.2 初始化timer;
 *					// void hrtimer_init(struct hrtimer *timer, clockid_t clock_id, enum hrtimer_mode mode)
 *					hrtimer_init(&on_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
 * 					hrtimer_init(&on_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
 *						+ CLOCK_MONOTONIC（推荐）：单调递增的时间（不会因系统时间调整而回退）;
 *						+ HRTIMER_MODE_REL: 以当前时间为基准，延迟一段时间后触发定时器;
 *				5.3 指定定时任务，即回调函数;
 *					on_timer.function = on_timer_cb;
 * 					off_timer.function = off_timer_cb;
 *					...
 *					static enum hrtimer_restart monitor_handler(struct hrtimer *hrtimer)
 *						+ monitor_handler: 
 *							+ content:
 *							+ return: enum hrtimer_restart类型，决定了timer是:
 *								+ HRTIMER_NORESTART,  // 只触发一次（单次定时器）
 *								+ HRTIMER_RESTART     // 继续运行（周期性定时器）
 * 					+ on_timer_cb:
 * 						static enum hrtimer_restart on_timer_cb(struct hrtimer *hrtimer)
 * 						{
 * 							enum hrtimer_restart ret = HRTIMER_NORESTART;
 * 							gpiod_set_value(led_gpio, 1); // 点亮
 * 							hrtimer_start(&off_timer, ms_to_ktime(100), HRTIMER_MODE_REL);
 * 							return ret;
 * 						}
 * 					+ off_timer_cb:
 * 						static enum hrtimer_restart off_timer_cb(struct hrtimer* hrtimer)
 * 						{
 * 							gpiod_set_value(led_gpio, 0);	// 熄灭
 * 							if (led_counter == 1 || led_counter == 10)
 * 								led_step *= -1;
 * 							led_counter += led_step;
 * 							hrtimer_start(&on_timer, TIMES_INTERVAL(led_counter), HRTIMER_MODE_REL);
 * 							return ret;
 * 						}
 * 					+ 两个回调函数，都是单次定时器，但是他们彼此牵动，往返如此，形成不间断的定时;
 * 					+ off_timer 中控制了熄灭的时长，隔多久才去点亮 led;
 *				5.4 开启一个定时任务;
 *					// hrtimer_start_range_ns(struct hrtimer *timer, ktime_t tim, u64 delta_ns, const enum hrtimer_mode mode);
 *					hrtimer_start_range_ns(&on_timer, TIMES_INTERVAL(2), 0, HRTIMER_MODE_REL);
 *						+ ktime_t tim: 定时器的超时时间;
 *						+ u64 delta_ns: 定时器可接受的最大延迟，单位是纳秒 (ns)。设为 0 表示无延迟，即精确触发。
 *						+ HRTIMER_MODE_REL_PINNED: 定时器模式，相对当前时间启动，并固定到当前 CPU，即该定时器只会在当前 CPU 上触发，不会迁移到其他 CPU。
 * 						+ HRTIMER_MODE_REL: 定时器模式，相对当前时间启动;
 * 					// static inline void hrtimer_start(struct hrtimer *timer, ktime_t tim, const enum hrtimer_mode mode) {
 * 					// 		hrtimer_start_range_ns(timer, tim, 0, mode); }
 * 					hrtimer_start(&on_timer, TIMES_INTERVAL(led_counter), HRTIMER_MODE_REL);
 * 						+ 同: hrtimer_start_range_ns(&on_timer, TIMES_INTERVAL(led_counter), 0, HRTIMER_MODE_REL);
 * 						+ 即: 运行到hrtimer_start_rrange_ns的时间，相对间隔 TIMES_INTERVAL(led_counter)，不允许有延迟，精确执行定时器;
 *				5.5 取消一个定时任务;
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
 * 				cd /sys/firmware/devicetree/base/(设备树的根目录)，看看有没有创建的新节点 zlj_led_
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
 * 		<1>
 * 		printk("led_release: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
 * 		这句打印，不适合封装成一个函数调用，不然打印出来的__FUNCTION__和__LINE__都是封装函数的位置;
 * 		跟你原来的意愿（想去知道哪个函数进行的该printk）就违背了;
 * 		想使用这句，就直接将printk写入该函数;
 * 		
 * 		<2>
 * 		最开始，是在原来 led_drv3_dtbs 版本当中进行修改，当时 glitter 那个函数中，修改成:
 *      static void led_glitter(void)
 * 		{
 * 			
 * 			gpiod_set_value(led_gpio, 1);
 * 			//printk("time interval: %dms\n", led_counter * MIN_INTERVAL);
 * 			gpiod_set_value(led_gpio, 0);
 * 			
 * 			if (led_counter == 1 || led_counter == 10)
 * 				led_step *= -1;
 * 			led_counter += led_step;
 * 		}
 * 		设置完之后，发现灯不亮;
 * 		加了printk之后，验证有无设置成功，发现 led 灯是快速的完成一次闪烁;
 * 		猜想是时间间隔太短了，切换;
 * 		后面注释掉 printk，再打开对比，问题确实如此;
 * 		原来在一次亮灭之间进行延时，是不可行的: msleep(50)
 * 		msleep 在某个 不能被调度的上下文中 被执行了，而 msleep() 本质上会导致 schedule()，于是内核发出了警告：bad: scheduling from the idle thread!
 * 		在 idle thread（swapper/0）或者中断/定时器上下文中尝试睡眠，这种行为在 Linux 内核中是严重的逻辑错误。
 * 		
 * 		因为上面的方式不可行，嵌套的定时器是异步的，想要达到延时不可行;
 * 		所以，采取了两个单次定时任务，每次回调函数结束，调用另一方，形成不间断的定时;
 */
