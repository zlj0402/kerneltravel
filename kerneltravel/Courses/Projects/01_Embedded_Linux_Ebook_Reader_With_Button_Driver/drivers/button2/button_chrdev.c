#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/poll.h>
#include "button_core.h"

#define CHRDEV_NAME "reader_button"

static int major;

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

int button_chrdev_init(void)
{
    major = register_chrdev(0, CHRDEV_NAME, &button_fops);
    printk("button chrdev major: %d\n", major);
    return 0;
}

void button_chrdev_exit(void)
{
    unregister_chrdev(major, CHRDEV_NAME);
}

