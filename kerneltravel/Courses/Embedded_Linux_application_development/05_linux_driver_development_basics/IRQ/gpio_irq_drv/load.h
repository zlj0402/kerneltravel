/**
 * @brief: key button, request_irq; -- porting on 9/6/2025
 *
 * @chapter: Notion -- 
 * 				+ 第五篇：嵌入式Linux驱动开发基础知识
 * 					+ 【第十九章】Linux系统对中断的处理
 * 						+ 06_IMX6ULL设备树修改及上机实验
 *
 * @ steps:
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
 * 			1.3 定义上面两个引脚（按键引脚）的设备树节点;
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
 * 		3. 为设备树中声明的 GPIO 引脚，动态申请 GPIO 并注册为中断
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
 * 		4. steps
 * 			dtb 文件上面已经拷贝，重启开发板了;
 * 			4.1 编译 驱动程序;
 * 			4.2 .ko 文件拷贝到开发板上;
 * 			4.3 insmod .ko
 * 			4.4 按下开发板上的 key1, key2
 * 		5. 查看中断统计和分配情况：cat /proc/interrupts
 * /
