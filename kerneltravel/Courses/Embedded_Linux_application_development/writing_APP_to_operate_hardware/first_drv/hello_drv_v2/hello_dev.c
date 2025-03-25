#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/device.h>
#include <linux/ioctl.h>
#include <linux/parport.h>
#include <linux/ctype.h>
#include <linux/poll.h>
#include <linux/slab.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/uaccess.h>
#include <linux/compat.h>

#include <linux/platform_device.h>

/* 参考：Linux-4.9.88\arch\arm\mach-imx\mach-qong.c */

static struct resource hello_resources[] = {
	{
		.name	= "hello_device123",
		.start	= 0,
		.end	= 3,
		.flags	= IORESOURCE_MEM,
	}
};

void hello_dev_release(struct device *dev)
{
	
}

/* 分配/设置/注册 platform_device */
static struct platform_device hello_device = {
	.name			= "firt_hello_drv_detached",
	.dev = {
		.release = hello_dev_release,
	},
	.id			= -1,
	.num_resources		= ARRAY_SIZE(hello_resources),
	.resource		= hello_resources,
};

static int __init hello_dev_init(void)
{	
	int err;
	err = platform_device_register(&hello_device);
	return err;
}

static void __exit hello_dev_exit(void) 
{
	platform_device_unregister(&hello_device);
	printk(">>>>>>>>>>hello_dev_exit, ended!<<<<<<<<<<\n");
}

module_init(hello_dev_init);
module_exit(hello_dev_exit);

MODULE_LICENSE("GPL");

