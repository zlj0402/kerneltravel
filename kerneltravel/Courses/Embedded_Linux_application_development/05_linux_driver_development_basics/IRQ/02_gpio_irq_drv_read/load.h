/**
 * @brief: key button, request_irq with driver--read; -- porting on 23/6/2025
 *
 * @chapter: Notion -- 
 * 				+ 【第五篇】嵌入式Linux驱动开发基础知识
 * 					+ 【第二十章】驱动程序基石
 * 						+ 01_休眠与唤醒
 *
 * @steps:
 * 		1. dts 修改：
 * 			1.1 给 iomuxc_snvs 节点添加了一个 GPIO 配置（pinctrl） 节点 irq_key1_zlj，目的是将 MX6ULL_PAD_SNVS_TAMPER1（GPIO5_IO01）配置好，以便驱动里用作中断（IRQ）。
 * 				&iomuxc_snvs {
 * 				    pinctrl-names = "default_snvs";
 * 				    pinctrl-0 = <&pinctrl_hog_2>;
 *
 * 				    irq_key1_zlj : irq_key1_zlj {
 * 				    	fsl,pins = <
 * 				    		MX6ULL_PAD_SNVS_TAMPER1__GPIO5_IO01        0x000110A0
 * 				    	>;
 * 				    };
 * 				    ...
 * 				};
 * 				ps: irq_key1_zlj: → label
 * 					irq_key1_zlj { ... } → 节点名字
 * 			1.2 在设备树中添加了 GPIO4_IO14（即 MX6UL_PAD_NAND_CE1_B）引脚的定义，目的是在后续驱动里使用它作为中断。
 * 				&iomuxc {
 * 				    pinctrl-names = "default";
 * 				    pinctrl-0 = <&pinctrl_hog_1>;
 *
 * 				    irq_key2_zlj : irq_key2_zlj {
 * 				    	fsl,pins = <
 * 				    		MX6UL_PAD_NAND_CE1_B__GPIO4_IO14           0x000010B0
 * 				    	>;
 * 				   	};
 * 					...
 * 				};
 *  		1.3 定义上面两个引脚（按键引脚）的设备树节点; -- platform_device
 * 				    zlj_gpio_key {
 * 				    	compatible = "zlj,gpio_key";
 * 				    	pinctrl-names = "default";
 * 				    	pinctrl-0 = <&irq_key1_zlj &irq_key2_zlj>;
 *
 * 				    	gpios = <&gpio5 1 GPIO_ACTIVE_LOW
 * 				    			&gpio4 14 GPIO_ACTIVE_LOW>;
 * 				    };
 * 				    + compatible
 * 				    	+ 告诉内核用 zlj_gpio_key 驱动来匹配它（在驱动代码里，of_device_id 中会匹配 "zlj,gpio_key"）
 * 				    + pinctrl-names / pinctrl-0
 * 				    	+ 通过 pinctrl-0 = <&irq_key1_zlj &irq_key2_zlj>, 表示 zlj_gpio_key 节点有一个默认的状态，默认状态下，会使用 pinctrl-0 当中指定的两个节点中涉及到的引脚;
 * 				    	+ 驱动加载时会自动配置好两个引脚（GPIO5_IO01、GPIO4_IO14）的复用和 pad control
 * 				    		+ 内核在匹配到这个设备树节点后，会自动调用 pinctrl 子系统，帮你把这两个引脚配置好（复用到 GPIO 模式，设置电气特性）
 * 				    + gpios
 * 				    	+ 使用 <&gpio5 1 GPIO_ACTIVE_LOW> → GPIO5_IO01，低电平有效
 * 				    	+ 使用 <&gpio4 14 GPIO_ACTIVE_LOW> → GPIO4_IO14，低电平有效
 * 				    	+ 驱动里用 of_get_named_gpio() 或者 gpiod_get_array() 等方式可以拿到这些 GPIO
 * 			1.4 把原来的 GPIO 按键禁止掉;
 * 				    gpio-keys {
 * 				        compatible = "gpio-keys";
 * 				        pinctrl-names = "default";
 * 				        status = "disabled";
 *
 * 				        user1 {
 * 				        	label = "User1 Button";
 * 				        	gpios = <&gpio5 1 GPIO_ACTIVE_LOW>;
 * 				        	gpio-key,wakeup;
 * 				        	linux,code = <KEY_1>;
 * 				        };
 *
 * 				        user2 {
 * 				            label = "User2 Button";
 * 				            gpios = <&gpio4 14 GPIO_ACTIVE_LOW>;
 *				            gpio-key,wakeup;
 *				            linux,code = <KEY_2>;
 *				        };
 *				    };
 * 				    ps: 添加了 status = "disabled"; 来禁用原来的按键设置;
 * 		2. dts 编译等步骤：
 * 			2.1 修改好 Linux-4.9.88/arch/arm/boot/dts/100ask_imx6ull-14x14.dts
 * 			2.2 回到 Linux-4.9.88 内核的根目录;
 * 			2.3 执行 make dtbs
 * 			2.4 将新生成的 100ask_imx6ull-14x14.dtb 拷贝到开发板上;
 * 			2.5 将 dtb 文件拷贝到 /boot 目录下;
 * 			2.6 重启开发板;
 * 		3. 为设备树中声明的 GPIO 引脚，动态申请 GPIO 并注册为中断 -- platform_driver 部分（具体是platfrom_driver.probe）
 * 			3.1 获取设备树节点
 * 				+ struct device_node *node = pdev->dev.of_node;
 * 					+ pdev->dev.of_node → 获取当前设备对应的 设备树节点（zlj_gpio_key）
 * 					+ 这个节点里保存了 gpios、pinctrl-0 等信息
 * 			3.2 遍历每个 GPIO
 * 				+ for (i = 0; i < count; i++) {
 * 	 				gpio = of_get_gpio_flags(node, i, &flags);
 * 		 				+ of_get_gpio_flags()：
 * 		 				+ 取出索引 i 的 GPIO 编号 gpio（如 129, 110…）
 * 		 				+ 解析是否设置了 GPIO_ACTIVE_LOW（低电平有效）
 * 		 	3.3 将 GPIO 转换成中断号
 * 		 		+ irq = gpio_to_irq(gpio);
 * 					+ gpio_to_irq() → 把 GPIO 编号映射成 中断号，供 request_irq 使用
 * 			3.4 申请 GPIO
 * 				+ if (flags & OF_GPIO_ACTIVE_LOW) {
 * 	 				flags |= GPIOF_ACTIVE_LOW;
 * 	 			 }
 *
 * 		 		err = devm_gpio_request_one(&pdev->dev, gpio, flags, NULL);
 * 		 			+ 如果设备树里声明了 GPIO_ACTIVE_LOW，则在申请 GPIO 时也带上
 *  	 			+ devm_gpio_request_one() → 让内核管理这个 GPIO（自动释放）
 * 		 	3.5 申请中断
 * 		 		+ err = request_irq(irq, gpio_key_irq_zlj, IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING, "zlj_gpio_keys", &gpio_keys[i]);
 * 					+ request_irq() → 注册中断处理函数
 * 		 			+ gpio_key_irq_zlj：中断处理函数
 * 		 			+ 触发方式：双沿触发（按下/松开都触发）
 * 		 			+ "zlj_gpio_keys"：中断名字（随便写）
 * 		 			+ &gpio_keys[i]：在中断触发时，会作为参数传给 gpio_key_irq_zlj
 * 		--------------------------------------------------------------------------
 * 		4. 注册驱动程序 & 更新中断服务程序 with wait_queue
 * 			4.1 注册驱动程序;
 * 				4.1.1 实现 file_operations 结构体的内容，本节重点是 file_operations.read 
 *  					ssize_t gpio_key_drv_read(struct file *file, char __user *buf, size_t size, loff_t *off) {
 * 		 					int err;
 * 		 					wait_event_interruptible(gpio_key_wait, g_key);
 * 		 					err = copy_to_user(buf, &g_key, sizeof(int));
 * 		 					g_key = 0;
 * 	 					
 * 		 					return sizeof(int);
 * 		 				}
 * 		 				static struct file_operations gpio_key_drv = {
 * 		 					.owner = THIS_MODULE,
 * 		 					.read = gpio_key_drv_read,
 * 		 				}
 * 		 			重点:
 * 		 				+ wait_event_interruptible(gpio_key_wait, g_key);
 * 		 					+ 检查条件 g_key 是否为真;
 * 		 					+ 如果条件为假（g_key == 0），当前进程会被挂起（睡眠），加入到 gpio_key_wait 这个等待队列中;
 * 		 					+ 如果条件为真，就直接返回;
 * 		 				+ 在中断服务程序当中，会调用相对应的 wakeup 函数;
 * 		 		4.1.2 platform_driver.probe 当中注册字符驱动程序; & 并创建节点;
 * 		 		4.1.3 platform_driver.remove 当中销毁创建的节点; & 注销字符驱动程序;
 * 		 	4.2 更新中断服务程序 with wait_queue
 * 		 		前面在 file_operations.read 当中进行了用户进程的挂起 wait_event_interruptible(gpio_key_wait, g_key);，
 *				唤醒的核心是 wake_up_interruptible(&gpio_key_wait):
 *					+ 这个函数不会自动执行，需要你在中断或其他上下文里手动调用它，告诉内核：
 *						| “我修改了可能让条件变成真的数据（如 g_key），现在请去检查等待这个条件的进程，并唤醒它们。”
 *					+ 此处是中断的服务程序;
 *			>>> 整个过程的流程梳理 <<<
 *			a. 用户进程调用 read()，进入 gpio_key_drv_read()
 *				wait_event_interruptible(gpio_key_wait, g_key);
 *					+ g_key == 0，所以当前进程被挂起，加入 gpio_key_wait 等待队列。
 *			b. 	硬件按键触发中断，中断处理函数运行:
 *				static irqreturn_t gpio_key_irq_zlj(int irq, void* dev_id) { ...
 *					+ 这一步是关键点，它显式告诉内核:
 *						“条件可能变了（g_key 改了），检查一下等待队列里有没有人等着，去唤醒他们。”
 *			c. 内核从 gpio_key_wait 中找到之前阻塞的进程，唤醒它:
 *				+ 被唤醒后，wait_event_interruptible() 会重新检查条件 g_key != 0 是否成立;
 *				+ 成立就继续执行;
 *				+ 否则（理论上不成立的概率很小），会再次睡眠;
 *			>>>>>>>>>>> end <<<<<<<<<<<
 * 		5. steps
 * 			dtb 文件上面已经拷贝，重启开发板了;
 * 			5.1 编译 驱动程序;
 * 			5.2 .ko 文件拷贝到开发板上;
 * 			5.3 insmod .ko
 * 			5.4 按下开发板上的 key1, key2
 * 			5.5 执行 ./button_irq_test /dev/zlj_gpio_key
 * 				+ 5.4 步骤的时候，我们能看到 中断服务程序的输出:
 * 					[14330.841750] key 189 val 0
 * 					[14331.017738] key 189 val 1
 * 					[14331.321100] key 189 val 0
 * 					[14331.513510] key 189 val 1
 * 				+ 执行测试程序的输出:
 * 					get button : gpio 110 val 0
 * 					只显示了，最后一次得到的 val;
 * 					命名中断事件有 4 次，我们只得到了最后一次;
 * 					>>> 后面一节会加上循环队列的方式，获取中断的按键值 <<<
 * 		6. 查看中断统计和分配情况：cat /proc/interrupts
 */
