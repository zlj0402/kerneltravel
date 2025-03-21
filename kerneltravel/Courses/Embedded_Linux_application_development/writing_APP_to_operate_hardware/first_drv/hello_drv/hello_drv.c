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

/* 参考：\Linux-4.9.88\drivers\char\ppdev.c */
#define BUFLEN 128
#define CHRDEV_NO 88
#define CHRDEV_NAME "zlj_first_drv"

static char hello_buf[BUFLEN] = "liangj.zhang@qq.com";

static int hello_open(struct inode *inode, struct file *file)
{
	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}

/* APP: read(fd, buffer, len) */
static ssize_t hello_read(struct file *file, char __user *buf, size_t count,
								loff_t *ppos)
{
	unsigned long err;
	if (count > BUFLEN)
		count = BUFLEN;
	// unsigned long copy_to_user(void __user *to, const void *from, unsigned long n)
	err = copy_to_user(buf, hello_buf, count);
	
	return count - err;
}

/* APP: write(fd, buffer, len) */
static ssize_t hello_write(struct file *file, const char __user *buf,
						size_t count, loff_t *ppos)
{
	unsigned long err;
	if (count > BUFLEN)
		count = BUFLEN;

	// unsigned long copy_from_user(void *to, const void __user *from, unsigned long n)
	err = copy_from_user(hello_buf, buf, count);

	return count - err;
}

static int hello_release(struct inode *inode, struct file *file)
{
	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}


/* 分配/设置/注册 file_operations结构体 */
// static -> 静态的，我们就不用去分配了
static const struct file_operations hello_fops = {
	.owner		= THIS_MODULE,
	.read		= hello_read,
	.write		= hello_write,
	.open		= hello_open,
	.release	= hello_release,
};
	
static int __init hello_init(void)
{
	int err;
	err = register_chrdev(CHRDEV_NO, CHRDEV_NAME, &hello_fops);
	printk("register_chrdev ret = %d\n", err);
	return err;
}

static void __init hello_exit(void) 
{
	// unregister_chrdev(unsigned int major, const char * name)
	unregister_chrdev(CHRDEV_NO, CHRDEV_NAME);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");



