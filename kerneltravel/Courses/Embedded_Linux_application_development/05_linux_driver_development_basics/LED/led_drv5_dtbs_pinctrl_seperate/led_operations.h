#ifndef LED_OPERATIONS_H
#define LED_OPERATIONS_H

#include <linux/device.h>

struct led_operations {
	int num;
	int (*init) (struct device*);
	int (*exit) (void);
	void (*direction_output) (int);
	void (*set_value) (int);
};

struct led_operations* get_imx6ull_led_opr(void);

#endif 	//LED_OPERATIONS_H
