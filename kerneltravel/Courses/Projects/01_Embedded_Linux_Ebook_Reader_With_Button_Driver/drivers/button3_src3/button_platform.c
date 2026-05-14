#include <linux/module.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include "button_core.h"
#include "load.h"

#define ONE_DAY_JIFFIES (24U * 60U * 60U * 100U)

struct gpio_key {
	
    int idx;
    int gpio;
    int irq;
    struct gpio_desc *gpiod;
	enum of_gpio_flags flags;
};

static struct gpio_key *gpio_keys;
static unsigned int g_last_key_time;
static unsigned int g_last_key_val;

static irqreturn_t gpio_key_irq_handler(int irq, void *dev_id)
{
	unsigned int key;
	unsigned int now;
    struct gpio_key *g_key = dev_id;
    int val = gpiod_get_value(g_key->gpiod);
	
	if (val == 0) {
		return IRQ_HANDLED;
	}
	
	now = jiffies % ONE_DAY_JIFFIES;
	key = (now << 8) | (g_key->gpio << 4) | (val << g_key->idx);
	printk("irq %d, gpio %d, val %d, now: %u, key %u\n\n", irq, g_key->gpio, val, now, key);
    button_core_put_key(&g_button_core, key);
	
	if (g_last_key_val && g_last_key_val != (val << g_key->idx)) {
		
		printk("last key time: %u now: %u, diff: %u\n", g_last_key_time, now, now - g_last_key_time);
	}
	
	g_last_key_time = now;
	g_last_key_val = val << g_key->idx;
	
    return IRQ_HANDLED;
}

static int gpio_irq_probe(struct platform_device *pdev)
{
	int err;
	enum of_gpio_flags flags;
    struct device_node *node = pdev->dev.of_node;
    int count = of_gpio_count(node);
    int i;
	int gpio;
	int irq;
	
	button_core_init(&g_button_core);   // 初始化核心
	
	if (!count) {
		printk("%s %s line %d, there isn't any gpio available\n", __FILE__, __FUNCTION__, __LINE__);
	} else {
		printk("gpio count: %d\n", count);
	}

    gpio_keys = devm_kzalloc(&pdev->dev, count * sizeof(*gpio_keys), GFP_KERNEL);
    for (i = 0; i < count; i++) {
		
		gpio = of_get_gpio_flags(node, i, &flags);
        irq = gpio_to_irq(gpio);

		gpio_keys[i].idx   = i;
        gpio_keys[i].gpio  = gpio;
        gpio_keys[i].gpiod = gpio_to_desc(gpio);
        gpio_keys[i].irq   = irq;
		gpio_keys[i].flags = flags;
		
		if (flags & OF_GPIO_ACTIVE_LOW) {
		    // 按键按下时电平为低，设置为低电平有效
    		flags |= GPIOF_ACTIVE_LOW;  // GPIOF_ACTIVE_LOW -> 在gpio系统中用来标识低电平有效
		}

        err = devm_gpio_request_one(&pdev->dev, gpio, GPIOF_IN, NULL);
		if (err < 0) {
			
			dev_err(&pdev->dev, "Failed to request GPIO %d, error %d\n",
				gpio, err);
			return err;
		}
		
		printk("\tgpio %d irq %d\n", gpio, irq);
		
        err = request_irq(irq, gpio_key_irq_handler, IRQF_TRIGGER_RISING, "zlj_gpio_key", &gpio_keys[i]);
    }
	
    return 0;
}

static int gpio_irq_remove(struct platform_device *pdev)
{
    struct device_node *node = pdev->dev.of_node;
    int count = of_gpio_count(node);
    int i;

    for (i = 0; i < count; i++)
        free_irq(gpio_keys[i].irq, &gpio_keys[i]);

    return 0;
}

static const struct of_device_id gpio_irq_table[] = {
    { .compatible = "button_driver,gpio_key" },
    { }
};

static struct platform_driver gpio_irq_drv = {
    .probe = gpio_irq_probe,
    .remove = gpio_irq_remove,
    .driver = {
        .name = "zlj_gpio_key",
        .of_match_table = gpio_irq_table,
    },
};

module_platform_driver(gpio_irq_drv);
MODULE_LICENSE("GPL");

