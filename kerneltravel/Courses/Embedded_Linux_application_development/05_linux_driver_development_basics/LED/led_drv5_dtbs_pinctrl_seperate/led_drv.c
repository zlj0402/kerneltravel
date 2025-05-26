#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/export.h>
#include <asm/uaccess.h>
#include <linux/device.h>
#include <asm/io.h>
#include <asm/page.h>
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/gpio/consumer.h>
#include <asm-generic/gpio.h>

#include "led_operations.h"
#include "led_hrtimer.h"
#include "load.h"

#define BUF_LEN 128
#define LED_CHRDEV_NAME "zlj_led"			// 注册的字符驱动程序的名称
#define LED_DEV_NAME "zljled"				// device_create，在 /dev/... 下面创建的设备名称
#define LED_DRV_CLASS_NAME "zlj_led_drv"	// class_create，为了在该 class 下面，创建该类型的 dev 设备于 /dev/... 下面
#define LED_DEVICE_NAME "zlj_led_drv_"		// 如果不是设备树生成的节点，.driver.name 是用于与手动注册的 platform_device.name 进行匹配的关键字段；而如果是设备树生成的设备，.driver.name 不参与匹配，只起到标识用途，.of_match_table 才是唯一有效的匹配方式。
#define DT_VERSION_PROP "version"			// 为了从 platform_device 那里得到一个设备节点里的 version 属性值
#define DT_IDS_COMPATIBLE "zlj_led_drv"		// 为了 platform_driver.driver.of_match_table 能够匹配设备树节点中 platfomr_device 的 compatible 属性值


static int major;
static struct class* led_drv_class;
static struct led_operations* p_led_opr;
static struct led_hrtimer led_timer;

static char led_ver_buf[BUF_LEN];

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> file_operaions <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

/* enable gpio
 * config pin as gpio
 * config gpio as output 
 */
static int led_fpos_open(struct inode *inode, struct file *file)
{
	p_led_opr->direction_output(0);
	
	return 0;
}

static int led_fops_release(struct inode *inode, struct file *file)
{
	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}

static ssize_t led_fpos_write(struct file *file, const char __user *buf,
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
	
	/* to set gpio data register : output 1/0 */
	if (val)
	{
		led_hrtimer_start(&led_timer);
		printk("start timer!!!\n");
	}
	else
	{
		led_hrtimer_stop(&led_timer);
		printk("end timer!!!\n");
		
		/* set gpio to let led off */
		p_led_opr->set_value(0);
	}

	return err;
}

static const struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.write = led_fpos_write,
	.open = led_fpos_open,
	.release = led_fops_release,
};

static int led_probe(struct platform_device *pdev)
{
	int len;
	int err;
	const char* vers;

	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);

	/* 4.1 设备树中定义有: led-gpios=<...>; */
	p_led_opr = get_imx6ull_led_opr();
	p_led_opr->init(&pdev->dev);

	// 设置 led_hrtimer
	led_hrtimer_init(&led_timer, p_led_opr);
	
	/* 从匹配的platform_device 当中得到版本信息 */
	err = of_property_read_string_index(pdev->dev.of_node, DT_VERSION_PROP, 0, &vers);
	if (err < 0)
		vers = "no version";
	len = strlen(vers) + 1;
	if (len > BUF_LEN)
		len = BUF_LEN;
	strncpy(led_ver_buf, vers, len);
	led_ver_buf[len - 1] = '\0';
	printk("Got led device version: %s\n", led_ver_buf);

	/* 注册led的字符驱动程序 */
  	// static inline int register_chrdev(unsigned int major, const char *name, const struct file_operations *fops)
	major = register_chrdev(0, LED_CHRDEV_NAME, &led_fops);
	if (major <= 0)
	{
		printk("register error.\n");
		return major;
	}
	else
	{
		printk("Got register_chrdev major: %d\n", major);
	}

	

	/* 创建led的设备节点 */
	led_drv_class = class_create(THIS_MODULE, LED_DRV_CLASS_NAME);
	if (IS_ERR(led_drv_class))
		return PTR_ERR(led_drv_class);
	device_create(led_drv_class, NULL, MKDEV(major, 0), NULL, LED_DEV_NAME);

	return 0;
	
}

static int led_remove(struct platform_device *pdev)
{
	led_hrtimer_stop(&led_timer);

	device_destroy(led_drv_class, MKDEV(major, 0));
	class_destroy(led_drv_class);

	/* 注销字符驱动程序 */
	unregister_chrdev(major, LED_CHRDEV_NAME);
	p_led_opr->exit();

	return 0;
}

static const struct of_device_id led_dt_ids[] = {
	{ .compatible = DT_IDS_COMPATIBLE, },
	{ /* sentinel */ }
};

static struct platform_driver led_driver = {
	.driver = {
		.name = LED_DEVICE_NAME,
		.of_match_table = led_dt_ids,
	},
	.probe = led_probe,
	.remove = led_remove,
};
		
static int __init led_drv_init(void)
{
	int err;
	err = platform_driver_register(&led_driver);
	printk(">>>>>>>>>> led platform_driver registered, ended! <<<<<<\n");
	printk("%s %s led_drv module loaded. \n", __FILE__, __FUNCTION__);
	return err;
}

static void __exit led_drv_exit(void)
{
	platform_driver_unregister(&led_driver);
	printk(">>>>>>>>>> led platform_driver unregistered, ended! <<<<<<\n");
	printk("%s %s led_drv module unloaded. \n", __FILE__, __FUNCTION__);
}

module_init(led_drv_init);
module_exit(led_drv_exit);

MODULE_LICENSE("GPL");
