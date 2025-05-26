#include <linux/gpio/consumer.h>
#include <linux/device.h>
#include <linux/of_gpio.h>

#include "led_operations.h"

#define GPIO_PINS_PER_BANK    32


static struct gpio_desc* led_gpio;


static int imx6ull_led_gpiod_init(struct device* dev)
{
	int gpio_num;
	int bank = -1;
	int offset = -1;
	
	/* 4.1 设备树中定义有: led-gpios=<...>; */
	/* gpiod_get 第 3 个参数，probe 里面并不指定使用哪个引脚，因为 probe 时并不会使用到引脚，用到时再设置 */
	led_gpio = gpiod_get(dev, "led", 0);
	if (IS_ERR(led_gpio)) {
		dev_err(dev, "Failed to get GPIO for led\n");
		return PTR_ERR(led_gpio);
	}

	/* 得到   led_gpio 对应的引脚号 */
	gpio_num = of_get_named_gpio(dev->of_node, "led-gpios", 0);
	printk("gpio_num: %d\n", gpio_num);
	if (!gpio_is_valid(gpio_num)) {
	    pr_err("Invalid GPIO\n");
	    return -EINVAL;
	}
	bank = gpio_num / GPIO_PINS_PER_BANK + 1;
	offset = gpio_num % GPIO_PINS_PER_BANK;
	printk("Got led_gpio: GPIO%d_%d\n", bank, offset);
	
	return 0;
}

static int imx6ull_led_gpiod_exit(void) 
{
	gpiod_put(led_gpio);

	printk("\n%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}

static void imx6ull_led_gpiod_direction_ouput(int direction) 
{
	/* 1. gpio module enabled defualtly */
	/* 2. already configured as gpio mode in device tree & pinctrl */
	/* 3. already get gpio pin in file_ops.probe: led_gpio = gpiod_get(&pdev->dev, "led", 0); */
	
	/* 4. config gpio as output */
	gpiod_direction_output(led_gpio, direction);
	
	printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
}

static void imx6ull_led_gpiod_set_value(int value)
{
	// 0: 逻辑值，熄灭;
	gpiod_set_value(led_gpio, value);
	
	// printk("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
}

/* 如果多个 led_gpio，可以使用这个函数; 一个的话，我们直接为该 led_gpio 设置 设置值 的函数;
static int board_demo_led_ctl(int which, char status)
{
	
	return 0;
}
*/

struct led_operations imx6ull_led_opr = {
	.num = 1,
	.init = imx6ull_led_gpiod_init,
	.exit = imx6ull_led_gpiod_exit,
	.direction_output = imx6ull_led_gpiod_direction_ouput,
	.set_value = imx6ull_led_gpiod_set_value,
};


struct led_operations* get_imx6ull_led_opr(void)
{
	return &imx6ull_led_opr;
}

