#ifndef BUTTON_OPERATIONS_H
#define BUTTON_OPERATIONS_H

#include <linux/of_gpio.h>
#include <linux/wait.h>

#define MAXGKEYS 128

extern struct fasync_struct *button_fasync;
extern wait_queue_head_t gpio_key_wait;

struct gpio_key {

	int idx;
	int gpio;
	int irq;
	struct gpio_desc* gpiod;
	enum of_gpio_flags flag;
};

struct button_operations {

	int count;
	int (*init) (int which); 	/* 初始化 button */
	int (*read) (int which);	/* 读取 button 对应引脚 */
};

void register_button_operations(struct button_operations* opr);
void unregister_button_operations(void);

#endif 	//BUTTON_OPERATIONS_H
