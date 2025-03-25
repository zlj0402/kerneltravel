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

/* 参考：\Linux-4.9.88\drivers\char\ipmi\ipmi_powernv.c */

#define BUFLEN 128
#define CHRDEV_NO 88
#define CHRDEV_NAME "zlj_first_drv"

static char hello_buf[BUFLEN] = "version1";

static int hello_open(struct inode *inode, struct file *file)
{
	printk("hello_open: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}

static size_t mstrlen(char* s)
{
	char* sc = s;
	for (; !*sc; sc++)
		/*nothing*/;
	return sc - s;
}

/* APP: read(fd, buffer, len) */
static ssize_t hello_read(struct file *file, char __user *buf, size_t count,
								loff_t *ppos)
{
	unsigned long err;
	if (count > BUFLEN)
		count = BUFLEN;
	
	printk("hello_read before copy_to_user, hello_buf: %s\n", hello_buf);
	// unsigned long copy_to_user(void __user *to, const void *from, unsigned long n)
	err = copy_to_user(buf, hello_buf, count);
	if (err) {
	    printk("hello_read: copy_to_user failed, err = %lu\n", err);
	    return -EFAULT;
	}
	printk("hello read: err = %lu\n", err);
	printk("---------\n");

	return mstrlen(hello_buf) - err;
}

static int hello_release(struct inode *inode, struct file *file)
{
	printk("hello_release: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}


/* 分配/设置/注册 file_operations结构体 */
// static -> 静态的，我们就不用去分配了
static const struct file_operations hello_fops = {
	.owner		= THIS_MODULE,
	.read		= hello_read,
	.open		= hello_open,
	.release	= hello_release,
};

static int hello_probe(struct platform_device *pdev)
{
	int err;
	int len;

	/**
	 * 从匹配的platform_device里得到版本信息
	 * 用来设置：hello_buf
	 */
	len = strlen(pdev->resource[0].name) + 1;
	if (len > 100) 
		len = 100;
	strncpy(hello_buf, pdev->resource[0].name, len);
	hello_buf[len - 1] = '\0';
	
	err = register_chrdev(CHRDEV_NO, CHRDEV_NAME, &hello_fops);
	printk(">>>>>>>>>>hello_probe, register_chrdev ret = %d<<<<<<<<<<\n", err);

	return err;
}

static int hello_remove(struct platform_device *pdev)
{
	// unregister_chrdev(unsigned int major, const char * name)
	unregister_chrdev(CHRDEV_NO, CHRDEV_NAME);

	return 0;
}


/* 分配/设置/注册一个 platform_driver */
static struct platform_driver hello_driver = {
	.driver = {
		.name		= "firt_hello_drv_detached",
	},
	.probe	= hello_probe,
	.remove	= hello_remove,
};


static int __init hello_init(void)
{	
	int err;
	err = platform_driver_register(&hello_driver);
	return err;
}

static void __exit hello_exit(void) 
{
	platform_driver_unregister(&hello_driver);
	printk(">>>>>>>>>>hello_exit, ended!<<<<<<<<<<\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");

