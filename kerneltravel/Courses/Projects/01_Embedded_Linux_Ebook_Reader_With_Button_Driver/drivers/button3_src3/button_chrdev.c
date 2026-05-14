#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/poll.h>
#include <linux/platform_device.h>
#include "button_core.h"

#define CHRDEV_CLS_NAME "reader_button_class"
#define CHRDEV_NAME "reader_button"

static int major;
static struct class *reader_button_class;

static ssize_t button_read(struct file *file, char __user *buf, size_t size, loff_t *off)
{
    int key;
    wait_event_interruptible(g_button_core.waitq, !button_core_empty(&g_button_core));
    key = button_core_get_key(&g_button_core);
    if (copy_to_user(buf, &key, sizeof(int)))
        return -EFAULT;
    return sizeof(int);
}

static unsigned int button_poll(struct file *file, poll_table *wait)
{
    poll_wait(file, &g_button_core.waitq, wait);
    return button_core_empty(&g_button_core) ? 0 : POLLIN | POLLRDNORM;
}

static const struct file_operations button_fops = {
	
    .owner = THIS_MODULE,
    .read = button_read,
    .poll = button_poll,
};

static int __init button_chrdev_init(void) {
	
    major = register_chrdev(0, CHRDEV_NAME, &button_fops);
    printk("button chrdev major: %d\n", major);
	
	reader_button_class = class_create(THIS_MODULE, CHRDEV_CLS_NAME);
	if (IS_ERR(reader_button_class)) {
		printk("%s class create failed.\n", CHRDEV_CLS_NAME);
		return PTR_ERR(reader_button_class);
	}
	else {
		printk("%s class create successfully.\n", CHRDEV_CLS_NAME);
	}
	device_create(reader_button_class, NULL, MKDEV(major, 0), NULL, CHRDEV_NAME);
    return 0;
}

static void __exit button_chrdev_exit(void) {
	
	device_destroy(reader_button_class, MKDEV(major, 0));
	class_destroy(reader_button_class);
    unregister_chrdev(major, CHRDEV_NAME);
}

module_init(button_chrdev_init);
module_exit(button_chrdev_exit);
MODULE_LICENSE("GPL");