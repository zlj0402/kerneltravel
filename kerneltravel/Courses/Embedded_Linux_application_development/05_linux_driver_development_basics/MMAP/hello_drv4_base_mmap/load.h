/**
 * @base_brief: 第一个 总线-设备-驱动分离模型 的驱动程序 -- 设备树方式构建platform_device -- 避免手动创建/dev/abcxyz;
 * @brief: 此次 mmap 的程序，是基于上面的 base_brief;
 *			1. 应用程序通过 mmap，将驱动程序中的物理地址映射到用户空间，得到一个用户空间的虚拟地址;
 *			2. 应用程序直接向这个虚拟地址写入数据;
 *			3. 写入的实际上是映射的内存区域，这个区域通常是内核态的缓冲区或物理内存;
 *			4. 你再通过 read 系统调用，从驱动中读取，验证用户态写入的数据是否真的写入到内核态缓冲区;
 *
 * @chapter: notion
 * 		第五篇：嵌入式Linux驱动开发基础知识
 * 			【第二十章】驱动程序基石
 * 				10_mmap编程
 *
 * @base_steps:
 * 		Bus-Device-Driver module
 * 		1. platform_device dtbs:
 * 			设备的树方式构建; 内核启动的时候，会去读入设备树，处理里面的节点信息，把里面的信息，转换为一个platform_device，然后注册进内核;
 * 			1.1 根据开发板类型，在Linux Kernel中找到相应位置的设备树文件，进行修改;
 * 				IMX6ULL开发板，在/Linux-4.9.88/arch/arm/boot/dts$ vim 100ask_imx6ull-14x14.dts
 * 			1.2 添加节点：
 * 				/ {
 * 					...
 *					hello {
 *						compatible = "first_zlj_drv";
 *						version = "version123456";
 *					}
 *					...
 * 				}
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
 * 					+ >>>>>>>>>>我们可以在probe这里，得到device的信息;<<<<<<<<<<
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
 * 			---------------------------------------------------
 * 			到这，platform_driver和之前platform_driver/platform_device都用.c文件来构建驱动模块没什么不同，
 * 			唯一一点不同的是，具体获取device信息的方式不同，需要相应的API
 * 			2.5 获得设备树节点构建的device信息;
 * 				platform_driver.probe的实现:
 * 					static int hello_probe(struct platform_device *pdev)
 * 					{
 * 						int err;
 * 						int len;
 * 						const char* vers = NULL;
 * 						...
 *						err = of_property_read_string_index(pdev->dev.of_node, "version", 0, &vers);
 *						if (err < 0)
 *							vers = "no version";
 *
 *						len = strlen(vers) + 1;
 * 						if (len > 100)
 * 							len = 100;
 * 							
 * 						strncpy(hello_buf, vers, len);
 * 						hello_buf[len - 1] = '\0';
 *						...
 * 					}
 * 				-----
 * 				of_property_read_string_index的使用:
 * 				+ 头文件包含:
 * 					#include <linux/of.h>
 * 				+ of_property_read_string_index 的 signment
 * 					static inline int of_property_read_string_index(const struct device_node *np,
 * 		 								const char *propname,
 * 		 								int index, const char **output)
	* 		 	+ of_property_read_string_index 的 作用及参数说明;
 * 					+ 作用:
* 						of_property_read_string_index() - Find and read a string from a multiple strings property.(用于读取具有多个string的属性)
 * 				+ 参数说明:
 * 					@np:		device node from which the property value is to be read.
 * 					@propname:	name of the property to be searched.
 * 					@index:	index of the string in the list of strings
 * 					@out_string:	pointer to null terminated return string, modified only if return value is 0.
 * 					Search for a property in a device tree node and retrieve a null
 * 					terminated string value (pointer to data, not a copy) in the list of strings
 * 					contained in that property.
 * 				+ 返回值:
 * 					Returns 0 on success, -EINVAL if the property does not exist, -ENODATA if
 * 					property does not have a value, and -EILSEQ if the string is not
 * 					null-terminated within the length of the property data.
 *
 * 				The out_string pointer is modified only if a valid string can be decoded.
 * 				-------------------------------------------------------------------------
 *			2.6 driver驱动程序里创建设备节点/dev/abcxyz
 *				+ 在platform_driver.probe注册了字符设备驱动之后，进行设备节点的创建:
 *					...
 *					hello_dev_class = class_create(THIS_MODULE, "hello");
 *					if (IS_ERR(hello_dev_class))
	*					return PTR_ERR(hello_dev_class);
	*				device_create(hello_dev_class, NULL, MKDEV(major_no, 0), NULL, "abcxyz");
	*				...
	*			+ 在platform_driver.remove中，进行设备节点的销毁:
	*				...
	*				// class detroy should be called after device_destroy
	*				device_destroy(hello_dev_class, MKDEV(major_no, 0));
	*				class_destroy(hello_dev_class);
	*				...
	*			这样，我们就可以在insmod hello_drv.ko插入模块之后（再次之前，设备树节点已经处理好了），直接执行测试程序;
 * @base_execute & steps:
 *		+ 设备树构建platform_device这块:
 *			我们已经添加过hello的设备树节点
 *			1. 在linux kernel根目录，执行make dtbs
 *			2. 将生成的dtb的二进制文件 arch/arm/boot/dts/100ask_imx6ull-14x14.dtb 到ubuntu和开发板共享的目录下;
 *			3. 开发板中，将dtb文件cp到/boot目录下，覆盖原来的dtb；然后重启
 *			4. 查看有没有生成设备树节点
 *				+ cd /sys/firmware/devicetree/base/(设备树的根目录)，看看有没有创建的新节点hello
 *					& 有的话，并进入，ls
 *					[root@100ask:/sys/firmware/devicetree/base/hello]# ls
 *					compatible  name  version
 *					有我们在设备树节点设置的属性
 *			5. 查看有没有生成platform_device
 *				+ cd /sys/bus/platform/devices
 *					有platform_device，但并没有对应的驱动程序;
 *					hello platform_device没有driver目录，表示还没有匹配的platform_driver；那是我们需要提供的platform_driver;
 *		+ .c文件来构建platform_driver模块:
 * 			1. Makfile中将模块和测试程序的编译都考虑到;
 * 			2. hello_drv.c hello_test.c Makefile等文件，相同路径下，执行make;
 * 			3. 将hello_drv.ko hello_test两个文件，拷贝到ubuntu和开发板的共享目录下;
 * 			4. 进入到开发板:
 * 			5. echo "7 4 1 7" > /proc/sys/kernel/printk, 修改内核日志的级别
 * 				这样，在终端上我们也能看到hello_drv.c当中的printk打印
 * 			6. insmod hell_drv.ko
 * 				插入模块
 * 			7. 执行测试程序，./hello_test r
 * 		of_match_table表示，能够支持哪些设备树当中的节点;
 * 		如果节点中的属性，跟.of_match_table当中的相匹配，就可支持;
 *
 * @base_execute & output:
 * 		[root@100ask:/mnt/04b_CodingAppOperatesHW/43_hello_drv_dtbs]# ./hello_test r
 * 		[ 8345.640100] hello_open: /home/zlj/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/writing_APP_to_operate_hardware/first_drv/hello_drv_v3_dtbs/hello_drv.c hello_open 28
 * 		[ 8345.657992] hello_read before copy_to_user, hello_buf: version123456
 * 		[ 8345.665513] hello read: err = 0
 * 		[ 8345.668684] ---------
 * 		len: 0, strlen: 14, get str from [ 8345.672546] hello_release: /home/zlj/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/writing_APP_to_operate_hardware/first_drv/hello_drv_v3_dtbs/hello_drv.c hello_release 63
 * 		drv: version123456
 *		可以看到，最后得到的版本信息，是我们在设备树节点中设置的version123456，
 *		不是在hello_drv.c当中设置的初始值"version1";
 *
 *	@execute & output: 见 notion @chapter;
 *
 *	@description:
 *		+ mmap MAP_SHARED: 应用程序是真正的写入了内核态的物理内存, 且是源数据;
 *		+ mmap MAP_PRIVATE: copy-on-write, COW 时分配的新匿名页，虽然物理内存是内核分配的，但它属于用户态地址空间，不是内核态地址；内核自身无法直接访问这些页。
 *				应用程序写入的是，分配的用户态地址空间的物理页，在 cp 的物理页上进行的修改;
 */
