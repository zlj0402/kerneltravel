#drivers/button/Makefile

ifneq ($(KERNELRELEASE),)
	ccflags-y += -I$(src)
	# 最终只生成两个 .ko
	obj-m := gpio_key_drv.o
	gpio_key_drv-objs := gpio_key_drv.o button_opr.o keys_queue.o gpio_key_chrdev.o

else
	KERNELDIR ?= /home/zlj/Codes/100ask_imx6ull-sdk/Linux-4.9.88
	PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

endif
