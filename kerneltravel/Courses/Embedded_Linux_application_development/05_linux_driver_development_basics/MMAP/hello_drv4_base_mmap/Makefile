# 如果已定义 KERNELRELEASE, 则说明是从内核构造系统调用的，
# 因此可利用其内建语句。
ifneq ($(KERNELRELEASE),)
	obj-m := hello_drv.o

# 否则，是直接从命令行调用的，
# 这时要调用内核构造系统。
else

	CC:=$(BUILDROOT_CROSS_COMPILE)gcc
	KERNELDIR ?= /home/zlj/Codes/100ask_imx6ull-sdk/Linux-4.9.88
	PWD := $(shell pwd)
	
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	$(CC) -o hello_test hello_test.c

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm -f hello_test

endif

