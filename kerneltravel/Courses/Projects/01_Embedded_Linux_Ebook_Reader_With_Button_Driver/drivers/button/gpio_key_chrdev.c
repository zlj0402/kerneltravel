// module_init...
#include <linux/module.h>
// copy_to_user
#include <asm/uaccess.h>
// poll_table
#include <linux/poll.h>

#include <keys_queue.h>
#include <button_opr.h>

#define CHRDEV_NAME "reader_button_chr"
#define BUTTON_CLASS_NAME "reader_button_cls"
#define BUTTON_DEV_NAME "reader_button_dev"

/* 主设备号 */
static int major;
static struct class* button_class;

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> file_operations & fops.read <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

ssize_t button_drv_read(struct file *file, char __user *buf, size_t size, loff_t *off) {

	int err;
	int key;
	
	wait_event_interruptible(gpio_key_wait, !kque->empty(kque));
	key = kque->get(kque);
	err = copy_to_user(buf, &key, sizeof(int));

	return sizeof(int);
}

static unsigned int button_drv_poll(struct file *fp, poll_table * wait) {

	poll_wait(fp, &gpio_key_wait, wait);

	return !kque->empty(kque) ? 0 : POLLIN | POLLRDNORM;
}

static int button_drv_fasync(int fd, struct file *file, int on) {

	if (fasync_helper(fd, file, on, &button_fasync) >= 0)
		return 0;
	else 
		return -EIO;
}

static struct file_operations button_drv = {
	
	.owner = THIS_MODULE,
	.read = button_drv_read,
	.poll = button_drv_poll,
	.fasync = button_drv_fasync,
};

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> module <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

static int __init button_chr_dev_init(void) {

	major = register_chrdev(0, CHRDEV_NAME, &button_drv);
 	if (major <= 0) {
		
		printk("register_chrdev err: %d.\n", major);
		return major;
	}
	else {
		printk("register_chrdev major: %d\n", major);
	}
	
	button_class = class_create(THIS_MODULE, BUTTON_CLASS_NAME);
	if (IS_ERR(button_class)) {

		printk("\n%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
		unregister_chrdev(major, BUTTON_CLASS_NAME);
		return PTR_ERR(button_class);
	}

	device_create(button_class, NULL, MKDEV(major, 0), NULL, BUTTON_DEV_NAME);

	printk(">>>>> %s -- %s, button_chrdev module loaded! <<<<<\n", __FILE__, __FUNCTION__);
	return 0;

}

static void __exit button_chr_dev_exit(void) {

	device_destroy(button_class, MKDEV(major, 0));
	class_destroy(button_class);

	unregister_chrdev(major, CHRDEV_NAME);
	
	printk(">>>>> %s -- %s, button_chrdev module unloaded! <<<<<\n", __FILE__, __FUNCTION__);
}

module_init(button_chr_dev_init);
module_exit(button_chr_dev_exit);

MODULE_LICENSE("GPL");

