#ifndef BUTTON_OPERATIONS_H
#define BUTTON_OPERATIONS_H

#include <linux/of_gpio.h>
#include <linux/wait.h>

// defined in keys_queue.c
extern struct keys_queue* kque;
extern struct button_operations* p_button_opr;

// defined in gpio_key_drv.c
extern wait_queue_head_t gpio_key_wait;
extern struct fasync_struct *button_fasync;


struct gpio_key {

	int idx;
	int gpio;
	int irq;
	struct gpio_desc* gpiod;
	enum of_gpio_flags flag;
};

struct button_operations {

	struct keys_queue* kque;
	void (*init) (struct button_operations *button_opr);
	void  (*read) (int irq, struct gpio_key *g_key);
	void (*set_keys) (struct device_node *node, int idx, struct gpio_key *g_key);
	void (*exit) (struct button_operations *button_opr);
};

struct button_operations* get_button_operations(void);

#endif 	//BUTTON_OPERATIONS_H
