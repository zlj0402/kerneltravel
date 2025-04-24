#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#include "button_drv.h"

#define BUTTON_OPR_NAME "zlj_button_opr"
#define BUTTON_CLASS_NAME "zlj_button_class"

static int major = 0;
static struct button_operations *p_button_opr;
static struct class *button_class;

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> file_operations <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

static int button_open(struct inode *inode, struct file *file)
{
	int minor = iminor(inode);
	p_button_opr->init(minor);
	return 0;
}

ssize_t button_read(struct file *file, char __user *buf, size_t size, loff_t *off)
{
	int err;
	int minor = iminor(file_inode(file));
	char level;
	
	level = p_button_opr->read(minor);
	err = copy_to_user(buf, &level, sizeof(char));
	return 1;
}


int button_release(struct inode *inode, struct file *file)
{
	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}

static struct file_operations button_opr = {
	.open = button_open,
	.read = button_read,
	.release = button_release,
};

// >>>>>>>>>>>>>>>>>>>>>>>>>>>> button operations <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

void register_button_operations(struct button_operations* opr)
{
	int i;
	
	p_button_opr = opr;
	
	for (i = 0; i < opr->count; i++)
	{
		device_create(button_class, NULL, MKDEV(major, i), NULL, "zlj_button%d", i);
		printk("device_create: zlj_button%d\n", i);
	}
}

void unregister_button_operations(void)
{
	int i;

	for (i = 0; i < p_button_opr->count; i++)
	{
		device_destroy(button_class, MKDEV(major, i));
	}
}


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>> module <<<<<<<<<<<<<<<<<<<<<<<

static int __init button_init(void)
{
	major = register_chrdev(major, BUTTON_OPR_NAME, &button_opr);
	if (major < 0) {
		printk("register_chrdev err: %d\n", major);
		return major;
	}
	else
	{
		printk("register_chrdev major: %d\n", major);
	}
	
	button_class = class_create(THIS_MODULE, BUTTON_CLASS_NAME);
	if (IS_ERR(button_class)) {
		printk("\n%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
		unregister_chrdev(major, BUTTON_CLASS_NAME);
		return PTR_ERR(button_class);
	}
	
	printk(">>>>> %s -- %s, button_drv module loaded! <<<<<\n", __FILE__, __FUNCTION__);
	return 0;
}

static void __exit button_exit(void)
{
	class_destroy(button_class);
	unregister_chrdev(major, BUTTON_OPR_NAME);
	printk(">>>>> %s -- %s, button_drv module unloaded! <<<<<\n", __FILE__, __FUNCTION__);
}

EXPORT_SYMBOL(register_button_operations);
EXPORT_SYMBOL(unregister_button_operations);

module_init(button_init);
module_exit(button_exit);

MODULE_LICENSE("GPL");
