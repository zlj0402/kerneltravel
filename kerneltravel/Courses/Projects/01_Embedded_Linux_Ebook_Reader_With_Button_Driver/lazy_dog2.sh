#!/bin/bash
set -e

make clean
make

SRC=$(make -s -f Makefile -pn | awk -F= '/^SRC[[:space:]]*:?=/{print $2}' | tail -n1 | xargs)
BUTTON_DIR_NAME=$(make -s -f Makefile -pn | awk -F= '/^BUTTON_DIR_NAME[[:space:]]*:?=/{print $2}' | tail -n1 | xargs)

# 目标目录
TARGET_DIR="/home/zlj/zljgit/kerneltravel/Courses/Projects/01_Embedded_Linux_Ebook_Reader_With_Button_Driver"
# 需要拷贝至的目录
COPY_DIR="/mnt/nfs_shared/Projects/01_Embedded_Linux_Ebook_Reader_With_Button_Driver/button_old"
# 需要拷贝的目标
OBJS="./$SRC/show_file ./drivers/$BUTTON_DIR_NAME/button.ko ./lazy_dog_board2.sh"

if [ "$PWD" = "$TARGET_DIR" ]; then
	echo "当前目录是 $PWD, 执行拷贝..."
	(set -x; sudo cp $OBJS $COPY_DIR)
	(set -x; ls -alt $COPY_DIR)
else
	echo "当前目录不是 $PWD, 跳过拷贝."
fi
