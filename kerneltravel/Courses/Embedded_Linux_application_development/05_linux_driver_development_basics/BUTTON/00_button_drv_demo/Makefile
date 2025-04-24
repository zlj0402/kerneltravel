obj-m += button_drv.o board_xxx.o

CC:=$(BUILDROOT_CROSS_COMPILE)gcc
CURRENT_PATH:=$(shell pwd)
KERNEL_DIR:=/home/zlj/Codes/100ask_imx6ull-sdk/Linux-4.9.88 

all:
	make -C $(KERNEL_DIR) M=$(CURRENT_PATH) modules
	$(CC) -o button_test button_test.c

clean:
	make -C $(KERNEL_DIR) M=$(CURRENT_PATH) clean
	rm -f button_test

