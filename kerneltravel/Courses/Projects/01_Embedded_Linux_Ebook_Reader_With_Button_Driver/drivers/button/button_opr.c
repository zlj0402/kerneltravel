// module_init / module_exit
#include <linux/module.h>
// struct device_node
#include <linux/of.h>
// of_gpio_flags
#include <linux/of_gpio.h>
// gpio_to_irq
#include <linux/gpio.h>
// gpio_to_desc
#include <linux/gpio/consumer.h>
// free
#include <linux/decompress/mm.h>

#include <keys_queue.h>
#include <button_opr.h>
#define MAXGKEYS 128

struct keys_queue* kque;
//EXPORT_SYMBOL(kque);

struct button_operations* p_button_opr;
//EXPORT_SYMBOL(p_button_opr);

static void button_operations_init(struct button_operations* button_opr) {

	button_opr->kque = keys_queue_create(MAXGKEYS);
}

static void button_operations_set_gpio_key(struct device_node *node, int idx, struct gpio_key *g_key) {

	int gpio;
	int irq;
	enum of_gpio_flags flags;

	gpio = of_get_gpio_flags(node, idx, &flags);
	irq  = gpio_to_irq(gpio);

	g_key->idx   = idx;
	g_key->gpio  = gpio;
	g_key->gpiod = gpio_to_desc(gpio);
	g_key->irq   = irq;
	g_key->flag  = flags;

	// 如果在设备树节点中，设置了低电平有效；OF_GPIO_ACTIVE_LOW -> 这是 .c 文件中用到的状态值
	// GPIOF_ACTIVE_LOW -> 这是在 gpio 系统中用到的状态值
	if (g_key->flag & OF_GPIO_ACTIVE_LOW) {	
		g_key->flag |= GPIOF_ACTIVE_LOW;
	}
}

static void button_operations_read_gpio(int irq, struct gpio_key *g_key) {

	int val;
	int key;

	val = gpiod_get_value(g_key->gpiod);
	printk("%s %d gpio_keys idx = %d, irq = %d, origin val = %d\n", 
		__FUNCTION__, __LINE__, g_key->idx, g_key->irq, val);

	// 接收到不同的 button，区别被按下的键值
	key = (g_key->gpio << 8) | (val << g_key->idx);
	kque->put(kque, key);
}

static void button_operations_exit(struct button_operations *button_opr) {

	if (!button_opr->kque)
		return;

	free(button_opr->kque);
	button_opr->kque = NULL;
}

static struct button_operations my_button_opr = {

	.init     = button_operations_init,
	.read     = button_operations_read_gpio,
	.set_keys = button_operations_set_gpio_key,
	.exit	  = button_operations_exit,
};

struct button_operations* get_button_operations(void) {

	my_button_opr.init(&my_button_opr);
	kque = my_button_opr.kque;
	p_button_opr = &my_button_opr;
	return p_button_opr;
}
//EXPORT_SYMBOL(get_button_operations);

MODULE_LICENSE("GPL");

