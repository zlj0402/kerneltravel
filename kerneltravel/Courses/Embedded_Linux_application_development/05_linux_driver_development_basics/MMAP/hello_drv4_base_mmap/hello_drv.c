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

#include <linux/kernel.h>
#include <linux/slab.h>
#include <asm/page.h>
#include <asm/io.h>
#include <asm/pgtable.h>
#include <linux/uio.h>
#include <linux/mm.h>

#include <linux/platform_device.h>
#include <linux/of.h>

/* 参考：\Linux-4.9.88\drivers\char\ipmi\ipmi_powernv.c */

#define BUFLEN 128
#define CHRDEV_NO 88
#define CHRDEV_NAME "zlj_first_drv"

static char *hello_buf;
static int buf_siz = 1024 * 8;
static struct class *hello_dev_class;
static int major_no;

static int hello_drv_open(struct inode *inode, struct file *file)
{
	//printk("hello_open: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
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
static ssize_t hello_drv_read(struct file *file, char __user *buf, size_t count,
								loff_t *ppos)
{
	unsigned long err;
	
	printk("hello_read before copy_to_user, hello_buf: %s\n", hello_buf);
	// unsigned long copy_to_user(void __user *to, const void *from, unsigned long n)
	err = copy_to_user(buf, hello_buf, count);
	if (err) {
	    printk("hello_read: copy_to_user failed, err = %d\n", min(buf_siz, (int)count));
	    return -EFAULT;
	}
	//printk("hello read: err = %lu\n", err);
	printk("---------\n");

	return mstrlen(hello_buf) - err;
}

static int hello_drv_mmap(struct file *file, struct vm_area_struct *vma)
{
	/* 获得物理地址 */
	unsigned long phys = virt_to_phys(hello_buf);

	/* 设置属性: cache, buffer -- able or not */
	vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);

	/* map */
	/* Remap-pfn-range will mark the range VM_IO */
	if (remap_pfn_range(vma,
			    vma->vm_start,
			    phys >> PAGE_SHIFT,
			    vma->vm_end - vma->vm_start,
			    vma->vm_page_prot)) {
		printk("mmap remap_pfn_range failed\n");
		return -EAGAIN;
	}

	return 0;
}

static int hello_drv_release(struct inode *inode, struct file *file)
{
	printk("hello_release: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}


/* 分配/设置/注册 file_operations结构体 */
// static -> 静态的，我们就不用去分配了
static const struct file_operations hello_fops = {
	.owner		= THIS_MODULE,
	.read		= hello_drv_read,
	.open		= hello_drv_open,
	.mmap 		= hello_drv_mmap,
	.release	= hello_drv_release,
};

static int hello_probe(struct platform_device *pdev)
{
	int err;
	int len;
	char major_no_buf[32] = "Failed";
	const char* vers = NULL;

	/**
	 * 从匹配的platform_device里得到版本信息
	 * 用来设置：hello_buf
	 * 以设备数匹配platform_device的话，我们怎么获取设备树节点当中的数据呢？
	 * A: 用某些专用的函数;
	 */
	/*static inline int of_property_read_string_index(const struct device_node *np,
						const char *propname,
						int index, const char **output)*/
	// index -> version属性值的第几个字符串;
	err = of_property_read_string_index(pdev->dev.of_node, "version", 0, &vers);
	if (err < 0)
		vers = "no version";
	
	len = strlen(vers) + 1;
	if (len > 100) 
		len = 100;
	strncpy(hello_buf, vers, len);
	hello_buf[len - 1] = '\0';

	major_no = CHRDEV_NO;
	err = register_chrdev(CHRDEV_NO, CHRDEV_NAME, &hello_fops);
	if (CHRDEV_NO != 0 && err < 0)
	{
		err = register_chrdev(0, CHRDEV_NAME, &hello_fops);
		if (err > 0)
			major_no = err;
		else 
			return err;
	}
	
	if (err >= 0 && major_no > 0) 
	{
		snprintf(major_no_buf, sizeof(major_no_buf), "%d", major_no);
	}

	hello_dev_class = class_create(THIS_MODULE, "hello");
	if (IS_ERR(hello_dev_class))
		return PTR_ERR(hello_dev_class);
	device_create(hello_dev_class, NULL, MKDEV(major_no, 0), NULL, "abcxyz");
	
	printk(">>>>>>>>>>hello_probe, register_chrdev major_no = %s, err = %d<<<<<<<<<<\n", 
		major_no_buf, err);

	return err;
}

static int hello_remove(struct platform_device *pdev)
{	
	// class detroy should be called after device_destroy
	device_destroy(hello_dev_class, MKDEV(major_no, 0));
	class_destroy(hello_dev_class);
	// unregister_chrdev(unsigned int major, const char * name)
	unregister_chrdev(major_no, CHRDEV_NAME);

	return 0;
}

static const struct of_device_id hello_dt_ids[] = {
	{ .compatible = "first_zlj_drv", },
	{ /* sentinel */ }
};



/* 分配/设置/注册一个 platform_driver */
static struct platform_driver hello_driver = {
	.driver = {
		.name		= "firt_hello_drv_detached",
		.of_match_table = hello_dt_ids,
	},
	.probe	= hello_probe,
	.remove	= hello_remove,
};


static int __init hello_init(void)
{	
	int err;
	hello_buf = kmalloc(buf_siz, GFP_KERNEL);
	strcpy(hello_buf, "old");
	if (!hello_buf) {
    	printk(KERN_ERR "kmalloc failed\n");
    	return -ENOMEM;
	}
	err = platform_driver_register(&hello_driver);
	return err;
}

static void __exit hello_exit(void) 
{
	platform_driver_unregister(&hello_driver);
	kfree(hello_buf);
	printk(">>>>>>>>>>hello_exit, ended!<<<<<<<<<<\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");

