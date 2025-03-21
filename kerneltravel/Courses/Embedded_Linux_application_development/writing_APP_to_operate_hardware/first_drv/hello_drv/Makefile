obj-m:=hello_drv.o

CC:=$(BUILDROOT_CROSS_COMPILE)gcc
CURRENT_PATH:=$(shell pwd)
KERNEL_DIR:=/home/zlj/Codes/100ask_imx6ull-sdk/Linux-4.9.88 

all:
	make -C $(KERNEL_DIR) M=$(CURRENT_PATH) modules
	$(CC) -o hello_test hello_test.c

clean:
	make -C $(KERNEL_DIR) M=$(CURRENT_PATH) clean
	rm -f hello_test

