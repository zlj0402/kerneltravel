#drivers/button/Makefile

ifneq ($(KERNELRELEASE),)
	ccflags-y += -I$(src)
	obj-m := gpio_key_drv.o gpio_key_chrdev.o keys_queue.o

else
	KERNELDIR ?= /home/zlj/Codes/100ask_imx6ull-sdk/Linux-4.9.88
	PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

endif
