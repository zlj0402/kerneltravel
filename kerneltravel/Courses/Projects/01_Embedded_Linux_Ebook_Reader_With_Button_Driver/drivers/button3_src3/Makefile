obj-m := button_platform.o button_core.o button_chrdev.o

# 内核源码目录
KERNELDIR := /home/zlj/Codes/100ask_imx6ull-sdk/Linux-4.9.88

# 当前模块目录
PWD := $(shell pwd)

# 默认目标
all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

# 清理
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
