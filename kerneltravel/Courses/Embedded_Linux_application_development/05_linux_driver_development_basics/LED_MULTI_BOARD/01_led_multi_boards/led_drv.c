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

#include "led_opr.h"


/* 参考：\Linux-4.9.88\drivers\char\ppdev.c */
#define CHRDEV_NO 88
#define CHRDEV_NAME "zlj_led_board"
#define LED_CLASS_NAME "zlj_led_board_class"
#define LED_DEV_NAME "zlj_led_board_dev%d"

MODULE_LICENSE("GPL");

static int major = CHRDEV_NO;
static struct led_operations* p_led_opr;
static struct class* led_class;


static int led_open(struct inode *inode, struct file *file)
{
	int minor = iminor(inode);

	printk("\n%s %s %d, minor: %d\n", __FILE__, __FUNCTION__, __LINE__, minor);
	/* 根据次设备号初始化LED */
	p_led_opr->init(minor);
	
	return 0;
}

/* APP: read(fd, buffer, len) */
static ssize_t led_read(struct file *file, char __user *buf, size_t count,
								loff_t *ppos)
{
	printk("\n%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}

/* APP: write(fd, buffer, len) */
static ssize_t led_write(struct file *file, const char __user *buf,
						size_t count, loff_t *ppos)
{
	unsigned long err;
	char status;
	int minor = iminor(file_inode(file));
	printk("\n%s %s %d, minor: %d\n", __FILE__, __FUNCTION__, __LINE__, minor);
	err = copy_from_user(&status, buf, 1);
	p_led_opr->ctl(minor, status);

	return err;
}

static int led_release(struct inode *inode, struct file *file)
{
	int minor = iminor(inode);
	printk("\n%s %s %d, minor: %d\n", __FILE__, __FUNCTION__, __LINE__, minor);
	p_led_opr->exit(minor);
	return 0;
}


/* 分配/设置/注册 file_operations结构体 */
// static -> 静态的，我们就不用去分配了
static const struct file_operations led_fops = {
	.owner		= THIS_MODULE,
	.read		= led_read,
	.write		= led_write,
	.open		= led_open,
	.release	= led_release,
};
	
static int __init led_init(void)
{
	int err;
	int i;
	
	err = register_chrdev(CHRDEV_NO, CHRDEV_NAME, &led_fops);
	if (err < 0) {
		printk("register_chrdev err: %d\n", err);
		return err;
	}

	led_class  = class_create(THIS_MODULE, LED_CLASS_NAME);
	if (IS_ERR(led_class)) {
		printk("\n%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
		unregister_chrdev(major, CHRDEV_NAME);
		return PTR_ERR(led_class);;
	} 
	else {
		printk("led_class is created.\n");
	}
	p_led_opr = get_board_led_opr();

	//device_create(led_class, NULL, MKDEV(major, 0), NULL, LED_DEV_NAME);
	for (i = 0; i < p_led_opr->num; i++) {
		device_create(led_class, NULL, MKDEV(major, i), NULL, LED_DEV_NAME, i);
	}

	printk(">>>>>>>>>>led_init, register_chrdev ret = %d<<<<<<<<<<\n", err);
	return err;
}

static void __exit led_exit(void) 
{
	int i;
	for (i = 0; i < p_led_opr->num; i++) {
		device_destroy(led_class, MKDEV(major, i));
	}
	class_destroy(led_class);
	
	// unregister_chrdev(unsigned int major, const char * name)
	unregister_chrdev(CHRDEV_NO, CHRDEV_NAME);
	printk(">>>>>>>>>>led_exit, ended!<<<<<<<<<<\n");
}

module_init(led_init);
module_exit(led_exit);


