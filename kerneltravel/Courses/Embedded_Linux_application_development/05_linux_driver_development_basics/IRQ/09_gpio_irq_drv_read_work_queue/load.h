/**
 * @brief: key button, request_irq with driver -- irq content in work_queue; -- porting on 1/7/2025
 *
 * @chapter: Notion -- 
 * 				+ 【第五篇】嵌入式Linux驱动开发基础知识
 * 					+ 【第二十章】驱动程序基石
 * 						+ 07_工作队列
 *
 * @descriptions:
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
 *			---------------------------
 *		5. 添加 drv_poll --- 第五篇 - 第二十章 - 02_POLL机制
 *			+ 我们通常理解的中断过程，没有消息时，手动挂起线程;
 *				但在 poll 机制中，挂起（即休眠）被内核封装在了 sys_poll 当中，我们只需要做两件事;
 *					+ drv_poll 将任务放入到 wait_queue 当中，但线程此时并未休眠;
 *					+ 返回是否有数据的状态;
 *				drv_poll 是被调用的，在代码中，被别人调用，就得遵守一定的规范;
 *				code:
 *					static unsigned int gpio_key_drv_poll(struct file *fp, poll_table * wait) {
 *						poll_wait(fp, &gpio_key_wait, wait);
 *						return is_key_buf_empty() ? 0 : POLLIN | POLLRDNORM;
 *					}
 *					+ poll_wait 将当前线程加入到 gpio_key_wait 等待队列;
 *						此时，只是加入到队列当中，线程还没有休眠;
 *					+ return，是否有消息的状态;
 *					---------------------------
 *		6. block or O_NONBLOCK to read --- 第五篇 - 第二十章 - 04_阻塞与非阻塞
 *			阻塞与非阻塞的方式，可以在打开设备节点时，也可以在之后，进行模式的设置;
 *				+ 打开设备节点时:
 *					int fd = open(“/dev/xxx”, O_RDWR | O_NONBLOCK); // 非阻塞方式
 *					int fd = open(“/dev/xxx”, O_RDWR ); // 阻塞方式
 *				+ 在打开设备节点后:
 *					int flags = fcntl(fd, F_GETFL);
 *					fcntl(fd, F_SETFL, flags | O_NONBLOCK); // 非阻塞方式
 *					fcntl(fd, F_SETFL, flags & ~O_NONBLOCK); // 阻塞方式
 *			drv_read 中，也可以根据 file 中记录的 f_flags，来做判断，要不要采用 wait_queue 的方式;
 *				ssize_t gpio_key_drv_read(struct file *file, char __user *buf, size_t size, loff_t *off) {
 *					int err;
 *					int key;
 *					
 *					if (is_key_buf_empty() && (file->f_flags & O_NONBLOCK))
 *						return -EAGAIN;
 *					wait_event_interruptible(gpio_key_wait, !is_key_buf_empty());
 *					
 *					key = get_key();
 *					err = copy_to_user(buf, &key, sizeof(int));
 *					return sizeof(int);
 *				}
 *				总结 drv_read 的效果:
 *					+ 不管模式如何，此种写法，会直接 return;
 *						如果在 while 中循环读，CPU 占用率会很高;
 *					+ 如果是 O_NONBLOCK,
 *						没数据，直接 return;
 *						有数据，wait_event_interruptible 不会将当前进程加入 wait_queue; 然后将数据拷贝给用户;
 *					+ 如果是默认的 block,
 *						没数据，wait_event_interruptible 将当前进程加入 wait_queue，等待中断服务程序唤醒;
 *						有数据，将数据拷贝给用户程序;
 *						-----------------------------
 *		7. irq content in timer handler --- 第五篇 - 第二十章 - 05_定时器
 *			由于金属弹片的机械振动，可能会使得一次按下按键，产生多次中断;
 *			消抖处理: 短时间（一定时长，自己设定，本例 2 个滴答）内中断多次产生，在这段时间内，重新修改定时时长，继续从此时延后 2 个滴答;
 *
 *			probe 中初始化 & 设置定时器:
 *				setup_timer(&gpio_keys[i].key_timer, key_timer_expire, (unsigned long)&gpio_keys[i]);
 *				gpio_keys[i].key_timer.expires = ~0;
 *				add_timer(&gpio_keys[i].key_timer);
 *
 *			定时器的 handler 函数(做的是原中断服务程序的内容):
 *				static void key_timer_expire(unsigned long data) {
 *					struct gpio_key* gpio_key = (struct gpio_key*) data;
 *					int val;
 *					int key;
 *					val = gpiod_get_value(gpio_key->gpiod);
 *					printk("%s gpio %d val %d\n", __FUNCTION__, gpio_key->gpio, val);
 *					key = (gpio_key->gpio << 8) | val;
 *					put_key(key);
 *					wake_up_interruptible(&gpio_key_wait);
 *				}
 *			现在的中断处理函数:
 *				static irqreturn_t gpio_key_irq_zlj(int irq, void* dev_id) {
 *					struct gpio_key* gkey = (struct gpio_key*) dev_id;
 *					printk("%s key %d val %d irq happened\n", __FUNCTION__, irq, gpio_get_value(gkey->gpio));
 *					mod_timer(&gkey->key_timer, jiffies + HZ / 50);
 *					return IRQ_HANDLED;
 *				}
 *
 *			+ 留一个疑问：中断之后，定时器已经存在，模块未卸载之前，会不会一直在执行定时器内容?
 *				不会，expire 赋值的是距离开机时间之后的一个时间点;
 *				--------------------------------------------------
 *		8. do something in tasklet(中断下半部) (上一节 timer 的设置没有删除) --- 第五篇 - 第二十章 - 06_中断下半部tasklet
 *			+ 在 gpio_key 结构体中，增加一个 tasklet_struct 属性;
 *				struct gpio_key {
 *					int gpio;
 *					struct gpio_desc* gpiod;
 *					int irq;
 *					enum of_gpio_flags flag;
 *					// 定时器
 *					struct timer_list key_timer;
 *					// tasklet
 *					struct tasklet_struct key_tasklet;
 *				};
 *			+ platform.probe 中对 gpio_keys 进行初始化的同时，进行 tasklet_init()
 *				for (i = 0; i < count; i++) {
 *					gpio = of_get_gpio_flags(node, i, &flags);
 *					irq = gpio_to_irq(gpio);
 *					...
 *					tasklet_init(&gpio_keys[i].key_tasklet, key_tasklet_func, (unsigned long)&gpio_keys[i]);
 *				}
 *				+ tasklet_init 参数
 *					+ 第一个是目标 tasklet 的地址
 *					+ 第二个参数是 下半部的内容函数
 *					+ 第三个参数是，下半部内容函数被调用时的参数;
 *			+ 在中断服务程序中，调度 tasklet
 *				tasklet_schedule(&gkey->key_tasklet);	
 *				+ 其实只是把 tasklet 放入队列当中;
 *				+ 真正被调度的时候，是每一次硬件中断之后，都会去处理软件中断，处理到 tasklet 这一类软件中断时，就会从tasklet 序列中，将每一个 tasklet 对象取出来，来执行 tasklet.func;
 *
 *			只有上面这三步骤，就结束了很简单...	
 *		9. do something in work_queue(工作队列，与用户进程共同竞争 CPU 资源，能休眠) --- 第五篇 - 第二十章 - 07_工作队列
 *			+ 在 gpio_key 结构体中，增加一个 work_struct 属性;
 *				struct gpio_key {
 *					int gpio;
 *					struct gpio_desc* gpiod;
 *					int irq;
 *					enum of_gpio_flags flag;
 *					// 定时器
 *					struct timer_list key_timer;
 *					// tasklet
 *					struct tasklet_struct key_tasklet;
 *					// work_queue
 *					struct work_struct key_work;
 *				};
 *			+ platform.probe 中对 gpio_keys 进行初始化的同时，进行 INIT_WORK
 *				for (i = 0; i < count; i++) {
 *					gpio = of_get_gpio_flags(node, i, &flags);
 *					irq = gpio_to_irq(gpio);
 *					...
 *					INIT_WORK(&gpio_keys[i].key_work, key_work_func);
 *				}
 *				+ INIT_WORK 宏的参数:
 *						+ 第一个参数是，目标 work_struct 属性的地址;
 *						+ 第二个参数是，内核线程从 work_queue 队列中取出的 wor_struct 对象的执行内容 .func;
 *			+ 在中断服务程序中，使用 schedule_work() 调度 work_struct;
 *				schedule_work(&gkey->key_work);
 *				+ 将 work_struct 对象加入系统默认的 work_queue;
 *				+ 再是将内核线程唤醒，去 work_queue 中一个一个 work 进行执行;
 *
 *			也是只有上面三步骤，work_queue 的使用就介绍完了;
 *			我们所做的也是简单的状态输出，缓冲池的数据写入，还是最开始的 block_read 方式;
 *							
 * @steps:
 * 		dtb 文件上面已经拷贝，重启开发板了;
 * 		1. 编译 驱动程序;
 * 		2. .ko 文件拷贝到开发板上;
 * 		3. insmod .ko
 * 		4. 按下开发板上的 key1, key2
 * 		5. 执行测试程序，按下按键;
 *			output:
 *				[24999.108381] key_tasklet_func gpio 110 val 1
 *				[24999.113831] key_work_func: the process is kworker/0:2 326
 *				[24999.119659] key_work_func gpio 110 val 1
 *				[24999.134684] key_timer_expire gpio 110 val 1
 *				[24999.305526] key_tasklet_func gpio 110 val 0
 *				[24999.310659] key_work_func: the process is kworker/0:2 326
 *				[24999.316339] key_work_func gpio 110 val 0
 *				[24999.334684] key_timer_expire gpio 110 val 0
 *			上面，是一次 按下中断 和一次 松开按键中断的输出，并输出内核线程的名称和pid;
 *			  326 root     [kworker/0:2]
 *			work_struct 当中只做了 printk 输出，没有做将数据放入缓冲池中，所以每次按下或松开，测试只输出一次按键引脚的状态;
 * 		6. 查看中断统计和分配情况：cat /proc/interrupts
 */
