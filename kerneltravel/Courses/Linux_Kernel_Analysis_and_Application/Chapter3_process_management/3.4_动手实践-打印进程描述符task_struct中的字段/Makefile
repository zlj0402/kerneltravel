#Makefile文件注意 : 假如前面的文件 .c 文件起名为 first.c, 那么这里的Makefile文件中的.o文件就要起名为first.o	只有root用户才能加载和卸载模块
obj-m:=task_struct.o			#产生task_struct模块的目标文件
# 目标文件	文件	要与模块名字相同
CURRENT_PATH:=$(shell pwd)		#模块所在的当前路径
LINUX_KERNEL:=$(shell uname -r)		#Linux内核代码的当前版本
LINUX_KERNEL_PATH:=/usr/src/linux-headers-$(LINUX_KERNEL)

all:
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) modules	#编译模块
#[Tab]	内核的路径	当前目录编译完放哪	表明编译的是内核模块

clean:
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) clean	#清理模块
