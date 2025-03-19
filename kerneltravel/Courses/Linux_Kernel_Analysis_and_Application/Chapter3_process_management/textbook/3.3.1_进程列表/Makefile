obj-m:=proclist_print.o

CURRENT_PATH:=$(shell pwd)
LINUX_KERNEL:=$(shell uname -r)		# Linux kernel version
LINUX_KERNEL_PATH:=/usr/src/linux-headers-$(LINUX_KERNEL)

all:
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) modules	# module compile

clean:
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) clean	# clean module
