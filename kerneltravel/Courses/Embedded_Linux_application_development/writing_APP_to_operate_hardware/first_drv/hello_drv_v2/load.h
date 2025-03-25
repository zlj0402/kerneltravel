/**
 * @brief: 第一个真正的驱动程序;
 *
 * @steps:
 * 		Bus-Device-Driver module
 * 		1. platform_device module:
 * 			也是写成模块:
 * 			1.1 模块入口函数, 注册platform_device
 * 				static int __init hello_dev_init(void)static int __init hello_dev_init(void)
 * 				{
 * 					return platform_device_register(&hello_device);
 * 				}
 * 			1.2 模块出口函数，注销platform_device
 * 				static void __exit hello_dev_exit(void) 
 * 				{
 * 					platform_device_unregister(&hello_device);
 * 				}
 * 			1.3 具体实现struct platform_device hello_device:
 * 				static struct platform_device hello_device = {
 * 					.name			= "firt_hello_drv_detached",
 * 					.dev = {
 * 						.release = hello_dev_release,
 * 					},
 * 					.id			= -1,
 * 					.num_resources		= ARRAY_SIZE(hello_resources),
 * 					.resource		= hello_resources,
 * 				};
 * 				+ .dev.release函数，是rmmod卸载模块时需要提供的，不然会有Warning
 * 				+ .name的名称，要与到时候得platform_driver结构体的.driver.name相匹配，
 * 					才会去调用platfrom_driver.probe，在里面进行驱动的register
 * 				+ .resource等其他的，就参考其他模块别写的;
 * 		2. platform_driver module:
 * 			写成模块形式：
 * 			2.1 模块入口函数，注册platform_driver
 * 				static int __init hello_init(void)
 * 				{
 * 					return platform_driver_register(&hello_driver);
 * 				}
 * 			2.2 模块出口函数，注销platform_driver
 * 				static void __exit hello_exit(void) 
 * 				{
 * 					platform_driver_unregister(&hello_driver);
 * 				}
 * 			2.3 具体实现struct platform_driver hello_driver;
 * 				static struct platform_driver hello_driver = {
 * 					.driver = {
 * 						.name		= "firt_hello_drv_detached",
 * 					},
 * 					.probe	= hello_probe,
 * 					.remove	= hello_remove,
 * 				};
 * 				+ 上面说了，只有相应的platform_device存在了，才会去调用.probe
 * 				+ .probe 函数的参数，就是一个struct platform_device*:
 * 					static int hello_probe(struct platform_device *pdev)
 * 					+ 我们可以在probe这里，得到device的信息;
 * 					+ 我们一般也要在probe当中，进行驱动的register:
 * 						register_chrdev(CHRDEV_NO, CHRDEV_NAME, &hello_fops);
 * 					+ 相应的，在remove当中，进行驱动的unregister;
 * 						unregister_chrdev(CHRDEV_NO, CHRDEV_NAME);
 * 			2.4 具体实现struct file_operations;
 * 				static const struct file_operations hello_fops = {
 * 					.owner		= THIS_MODULE,
 * 					.read		= hello_read,
 * 					.open		= hello_open,
 * 					.release	= hello_release,
 * 				};
 * 				+ .read就可以从用户态进程（普通进程，如hello_test程序的read(fd, buffer, len)）得到用户的数据;
 * 					static ssize_t hello_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
 * 				+ .open/.release等同样也是，对应相同的系统调用;
 *
 * @execute & steps:
 * 		1. Makfile中将模块和测试程序的编译都考虑到;
 * 		2. hello_drv.c hello_dev.c hello_test.c Makefile等文件，相同路径下，执行make;
 * 		3. 将hello_dev.ko hello_drv.ko hello_test三个文件，拷贝到ubuntu和开发板的共享目录下;
 * 		4. 进入到开发板:
 * 		5. echo "7 4 1 7" > /proc/sys/kernel/printk, 修改内核日志的级别
 * 			这样，在终端上我们也能看到hello_drv.c当中的printk打印
 * 		6. mknod /dev/abcxyz c 88 0
 * 			手动创建一个驱动节点
 * 		7. insmod hello_dev.ko
 * 			insmod hell_drv.ko
 * 			插入模块
 * 		8. 执行测试程序，./hello_test r
 * 			我们这里，在platform_device被注册的时候，platform_driver的name相匹配，就会调用platform.probe
 * 			我们在.probe当中进行了，将device.resource[0].name拷贝到hello_drv模块的全局变量hello_buf中;
 * 			在测试程序./hello_test r时，调用系统API read时，read hello_buf，
 * 			此时hello_buf已经不是当初hello_drv的初始值，而是从platform_device那边读取过来的;
 * 			-----
 * 			以后像硬件更新，咱就可以直接修改platform_device那边;
 *
 * @execute & output:
 * 		[root@100ask:/mnt/04b_CodingAppOperatesHW/41_hello_drv_detached]# ./hello_test r
 * 		[ 8106.895030] hello_open: /home/zlj/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/writing_APP_to_operate_hardware/first_drv/hello_drv_v2/hello_drv.c hello_open 27
 * 		[ 8106.911701] hello_read before copy_to_user, hello_buf: hello_device123
 * 		[ 8106.920337] hello read: err = 0
 * 		[ 8106.923682] ---------
 * 		len: 0, strlen: 16, get str from [ 8106.928105] hello_release: /home/zlj/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/writing_APP_to_operate_hardware/first_drv/hello_drv_v2/hello_drv.c hello_release 62
 * 		drv: hello_device123
 *
 * 		ps:
 * 			最后一个行drv: hello_device123这个输出，是printf的输出，被系统的输出插进来，分开了;
 * */
