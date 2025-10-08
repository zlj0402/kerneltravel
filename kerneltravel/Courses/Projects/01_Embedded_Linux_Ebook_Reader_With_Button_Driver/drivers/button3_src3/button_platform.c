#include <linux/module.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include "button_core.h"
#include "load.h"

struct gpio_key {
    int idx;
    int gpio;
    struct gpio_desc *gpiod;
    int irq;
};

static struct gpio_key *gpio_keys;

static irqreturn_t gpio_key_irq_handler(int irq, void *dev_id)
{
    struct gpio_key *key = dev_id;
    int val = gpiod_get_value(key->gpiod);
    unsigned int code = (jiffies << 8) | (key->gpio << 4) | val;

    printk("irq %d gpio %d val %d key %u\n", irq, key->gpio, val, code);
    button_core_put_key(&g_button_core, code);
    return IRQ_HANDLED;
}

static int gpio_irq_probe(struct platform_device *pdev)
{
	int err;
    struct device_node *node = pdev->dev.of_node;
    int count = of_gpio_count(node);
    int i;
	
	button_core_init(&g_button_core);   // 初始化核心

    gpio_keys = devm_kzalloc(&pdev->dev, count * sizeof(*gpio_keys), GFP_KERNEL);
    for (i = 0; i < count; i++) {
        int gpio = of_get_gpio(node, i);
        int irq = gpio_to_irq(gpio);

        gpio_keys[i].gpio = gpio;
        gpio_keys[i].gpiod = gpio_to_desc(gpio);
        gpio_keys[i].irq = irq;

        devm_gpio_request_one(&pdev->dev, gpio, GPIOF_IN, NULL);
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

