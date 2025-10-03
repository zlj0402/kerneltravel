#!/bin/bash
set -e

make clean
make

# 目标目录
TARGET_DIR="/home/zlj/zljgit/kerneltravel/Courses/Projects/01_Embedded_Linux_Ebook_Reader_With_Button_Driver"
COPY_DIR="/mnt/nfs_shared/Projects/01_Embedded_Linux_Ebook_Reader_With_Button_Driver/button_old"
OBJS="./src/show_file ./drivers/button_old2/gpio_irq_drv.ko ./lazy_dog_board.sh"

if [ "$PWD" = "$TARGET_DIR" ]; then
	echo "当前目录是 $PWD, 执行拷贝..."
	(set -x; sudo cp $OBJS $COPY_DIR)
	(set -x; ls -alt $COPY_DIR)
else
	echo "当前目录不是 $PWD, 跳过拷贝."
fi
