/**
 * @brief: 第一个真正的驱动程序;
 *
 * @steps:
 * 			1. 实现驱动程序的结构体，file_operations hello_fpos {
 * 										.owner = THIS_MODULE,
 * 										.open = hello_open,
 *										.read = hello_read,
 *										.write = hello_write,
 *										.release = hello_release,
 * 									};
 * 			2. 注册：
 * 				register_chrdev(n, &hello_drv, “name”)
 * 			3. 入口函数（里面，调用register注册）
 * 			4. 出口函数（里面，调用unregister_chrdev）
 *
 * @description:
 * 			1. 内核函数hello_write/hello_read这样，从用户态接收到的buf，不能直接操作它;
 * 				error notification: `BUG: recent printk recursion!`
 * 			2. 我们实现的内核函数hello_realease，是由close(fd)调用的;
 * 				+ 即使我们在hello_test.c，测试的应用程序中，没有主动调用close(fd);
 * 				  程序结束时，系统会自动调用close(fd)
 * 			3. 出口函数和内核函数的编译修饰符__init/__exit宏要设置正确;
 * 				static void __init hello_exit(void) 
 * 				{
 * 					// unregister_chrdev(unsigned int major, const char * name)
 * 					unregister_chrdev(CHRDEV_NO, CHRDEV_NAME);
 * 					printk(">>>>>>>>>>hello_exit, ended!<<<<<<<<<<\n");
 * 				}
 * 				退出函数的__exit，一开始我写成了__init，导致rmmod hello_drv模块时出错，如下：
 * 					[root@100ask:/mnt/04b_CodingAppOperatesHW/38_hello_drv]# rmmod hello_drv
 * 					[  237.551452] Unable to handle kernel paging request at virtual address 7f132054
 * 					[  237.559443] pgd = 8884c000
 * 					[  237.562220] [7f132054] *pgd=88771811, *pte=00000000, *ppte=00000000
 * 					[  237.570477] Internal error: Oops: 80000007 [#1] PREEMPT SMP ARM
 * 					[  237.576450] Modules linked in: hello_drv(O-) 8723bu(O) inv_mpu6050_spi inv_mpu6050 evbug 100ask_adxl345_spi 100ask_spidev 100ask_irda 100ask_rc_nec 100ask_dht11 100ask_ds18b20
 * 					...
 * 				上面的错误，就是因为这个原因;
 * 			4. 在模块入口函数register 驱动程序: register_chrdev(CHRDEV_NO, CHRDEV_NAME, &hello_fops);
 * 				也要记得在模块出口函数unregister 驱动程序: unregister_chrdev(CHRDEV_NO, CHRDEV_NAME);
 * 			5. CHRDEV_NAME -> 驱动程序的name并不重要，
 * 				只要我们的CHRDEV_NO号码，和<驱动设备的主设备号>相同就行;
 * 				凭借主设备号进行匹配的;
 *
 * @execute & steps:
 * 			1. ubuntu上配置好工具链，配置好和开发板同样的内核（编译、安装内核，不是替换ubuntu虚拟机本身的内核）
 * 			2. hello_drv.c hello_test.c文件路径下，执行make
 * 			3. hello_drv.ko和./hello_test拷贝到，和开发板共享的目录下
 * 			4. 进入到开发板：
 * 			5. echo "7 4 1 7" > /proc/sys/kernel/printk
 * 				这样做，是修改内核日志的级别;
 * 				这样，在终端上我们也能看到hello_drv.c当中的printk打印
 * 			6. insmod hello_drv # 往内核中插入模块
 * 			7. 还不能立即执行测试程序./hello_test r/w
 * 				因为，我们在hello_test.c中的open驱动设备的节点，写死了为/dev/abcxyz，随便取的，只要主设备号相同，即可调用
 * 			8. mknod /dev/abcxyz c 88 0
 * 					<dev_name> <type> <main_no> <minor_no>
 * 				手动创建一个驱动节点
 * 			9. 就可以测试./hello_test r ...
 *
 * @execute & output:
 * 			[root@100ask:/mnt/04b_CodingAppOperatesHW/38_hello_drv]# ./hello_test r
 * 			[  135.904676] hello_open: /home/zlj/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/writing_APP_to_operate_hardware/first_drv/hello_drv/hello_drv.c hello_open 24
 * 			[  135.921053] hello_read before copy_to_user, hello_buf: liangj.zhang@qq.com
 * 			[  135.929936] hello read: err = 0
 * 			[  135.933147] ---------
 * 			len: 0, strlen: 20, get str from [  135.939778] hello_release: /home/zlj/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/writing_APP_to_operate_hardware/first_drv/hello_drv/hello_drv.c hello_release 77
 * 			drv: liangj.zhang@qq.com
 * 			[root@100ask:/mnt/04b_CodingAppOperatesHW/38_hello_drv]# ./hello_test w abc
 * 			[  141.667060] hello_open: /home/zlj/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/writing_APP_to_operate_hardware/first_drv/hello_drv/hello_drv.c hello_open 24
 * 			[  141.684315] hello_write before copy_from_user, hello_buf: liangj.zhang@qq.com
 * 			[  141.691496] hello_write after copy_from_user, err: 0, hello_buf: abc
 * 			write: get len: 0[  141.700852] hello_release: /home/zlj/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/writing_APP_to_operate_hardware/first_drv/hello_drv/hello_drv.c hello_release 77
 * 			[root@100ask:/mnt/04b_CodingAppOperatesHW/38_hello_drv]# ./hello_test r
 * 			[  148.905118] hello_open: /home/zlj/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/writing_APP_to_operate_hardware/first_drv/hello_drv/hello_drv.c hello_open 24
 * 			[  148.921438] hello_read before copy_to_user, hello_buf: abc
 * 			[  148.928197] hello read: err = 0
 * 			[  148.931377] ---------
 * 			len: 0, strlen: 4, get str from d[  148.936102] hello_release: /home/zlj/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/writing_APP_to_operate_hardware/first_drv/hello_drv/hello_drv.c hello_release 77
 *
 * @ps:
 * 		copy_to_user 和 copy_from_user的返回值：
 * 		+ copy_from_user:
 * 			0：全部数据复制成功 ✅
 * 			>0：表示未复制的字节数 ❌（如 n=20，返回 5，说明只复制了 15 字节）
 * 		+ copy_to_user:
 * 			0：全部数据复制成功 ✅
 * 			>0：表示未复制的字节数 ❌（比如 n=10，返回 3，说明只复制了 7 字节）
 * */
