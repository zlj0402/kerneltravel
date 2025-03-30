#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/export.h>
#include <asm/uaccess.h>
#include <linux/device.h>
#include <asm/io.h>

#define LED_CHRDEV_NAME "zlj_led"
#define LED_DEV_NAME "zljled"

static int major;
static struct class* led_drv_class;

/* registers */

// IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3地址：0x2290000 + 0x14
static volatile unsigned long* IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3;

// GPIO5_GDIR地址：0x020AC004
static volatile unsigned long* GPIO5_GDIR;

// GPIO5_DR地址：0x020AC000
static volatile unsigned long* GPIO5_DR;

static ssize_t led_write(struct file *file, const char __user *buf,
			 size_t count, loff_t *ppos)
{
	unsigned int err;
	char val;
	/* copy_from user : get data from APP */
	err = copy_from_user(&val, buf, 1);
	if (err > 0)
	{
		printk("copy_from_user failed, uncopied character count(er)= %du\n", err);
		return -EFAULT;
	}
	
	/* to set gpio register : output 1/0 */
	if (val)
	{
		/* set gpio to let led on */
		*GPIO5_DR &= ~(1 << 3);	// 使引脚为低电平0
	}
	else
	{
		/* set gpio to let led off */
		*GPIO5_DR |= (1 << 3);	// 使引脚为高电平
	}

	return 1;
}

static int led_open(struct inode *inode, struct file *file)
{
	/* enable gpio
	 * config pin as gpio
	 * config gpio as output 
	 */
	 
	/* config pins as gpio mode*/
	*IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 &= ~0xf;
	*IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 |= 0x5;

	/* config gpio as output */
	*GPIO5_GDIR |= (1 << 3);
	
	printk("led_release: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	
	return 0;
}

static int led_release(struct inode *inode, struct file *file)
{
	printk("led_release: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}


static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.open = led_open,
	.write = led_write,
	.release = led_release,
};

static int __init led_init(void)
{
	// static inline int register_chrdev(unsigned int major, const char *name,
	// 										const struct file_operations *fops)
	major = register_chrdev(0, LED_CHRDEV_NAME, &led_fops);
	if (major < 0)
	{
		printk("register error.\n");
		return -1;
	}
	else
	{
		printk("Got register_chrdev major: %d\n", major);
	}

	led_drv_class = class_create(THIS_MODULE, "zlj_led_drv");
	if (IS_ERR(led_drv_class))
		return PTR_ERR(led_drv_class);
	device_create(led_drv_class, NULL, MKDEV(major, 0), NULL, LED_DEV_NAME);

	// IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3地址：0x2290000 + 0x14
	IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3 = ioremap(0x2290000 + 0x14, sizeof(unsigned long));

	// GPIO5_GDIR地址：0x020AC004
	GPIO5_GDIR = ioremap(0x020AC004, sizeof(unsigned long));

	// GPIO5_DR地址：0x020AC000
	GPIO5_DR = ioremap(0x020AC000, sizeof(unsigned long));
	
	return 0;
}

static void __exit led_exit(void)
{
	iounmap(IOMUXC_SNVS_SW_MUX_CTL_PAD_SNVS_TAMPER3);
	iounmap(GPIO5_GDIR);
	iounmap(GPIO5_DR);
	
	device_destroy(led_drv_class, MKDEV(major, 0));
	class_destroy(led_drv_class);
	// static inline void unregister_chrdev(unsigned int major, const char *name)
	unregister_chrdev(major, LED_CHRDEV_NAME);

	printk("led drv is removed.\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
