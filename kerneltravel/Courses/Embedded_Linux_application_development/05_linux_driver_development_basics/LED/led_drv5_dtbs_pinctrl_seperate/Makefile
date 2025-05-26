zlj_led_drv-y := chip_imx6ull_gpio.o led_drv.o
obj-m := zlj_led_drv.o

CC:=$(BUILDROOT_CROSS_COMPILE)gcc
CURRENT_PATH:=$(shell pwd)
KERNEL_DIR:=/home/zlj/Codes/100ask_imx6ull-sdk/Linux-4.9.88 

all:
	make -C $(KERNEL_DIR) M=$(CURRENT_PATH) modules
	$(CC) -o led_test led_test.c

clean:
	make -C $(KERNEL_DIR) M=$(CURRENT_PATH) clean
	rm -f led_test

