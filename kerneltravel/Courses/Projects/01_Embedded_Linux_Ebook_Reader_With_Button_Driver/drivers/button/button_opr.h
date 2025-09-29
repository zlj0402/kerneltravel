#ifndef BUTTON_OPERATIONS_H
#define BUTTON_OPERATIONS_H

#include <linux/of_gpio.h>
#include <linux/wait.h>

extern struct fasync_struct *button_fasync;
extern wait_queue_head_t gpio_key_wait;

struct gpio_key {

	int idx;
	int gpio;
	int irq;
	struct gpio_desc* gpiod;
	enum of_gpio_flags flag;
};

#endif 	//BUTTON_OPERATIONS_H
