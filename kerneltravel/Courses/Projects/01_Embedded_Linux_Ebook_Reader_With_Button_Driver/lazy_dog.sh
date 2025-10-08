#!/bin/bash
set -e

make clean
make

# 顶层 Makefile 中的 SRC 目录变量
SRC=$(make -s -f Makefile -pn | awk -F= '/^SRC[[:space:]]*:?=/{print $2}' | tail -n1 | xargs)
# 顶层 Makefile 中要编译的驱动模块路径 -- 某一个，like button2
BUTTON_DIR_NAME=$(make -s -f Makefile -pn | awk -F= '/^BUTTON_DIR_NAME[[:space:]]*:?=/{print $2}' | tail -n1 | xargs)
# 驱动模块的完整目录
BUTTON_DIR_PATH="./drivers/$BUTTON_DIR_NAME"

# 工具链编译的目标目录
TARGET_DIR="/home/zlj/zljgit/kerneltravel/Courses/Projects/01_Embedded_Linux_Ebook_Reader_With_Button_Driver"
# 需要拷贝至的目录
COPY_DIR="/mnt/nfs_shared/Projects/01_Embedded_Linux_Ebook_Reader_With_Button_Driver/button_old"
# 需要拷贝的目标
OBJS=("./$SRC/show_file" "./lazy_dog_board.sh" "./lazy_dog_board2.sh")

# 追加工具链编译出来的 .ko 文件
for ko in $(awk -F= '/^obj-m[[:space:]]*[:+]?=/ {
	line=$2
	gsub(/^[ \t]+|[ \t]+$/, "", line)
	n=split(line, arr, /[[:space:]]+/)
	for(i=1;i<=n;i++){
		mod=arr[i];
		gsub(/\.o$/, ".ko", mod);
		print mod
	}
}' $BUTTON_DIR_PATH/Makefile); do
	OBJS+=("$BUTTON_DIR_PATH/$ko")
done

# cp files to destination
if [ "$PWD" = "$TARGET_DIR" ]; then
	echo "当前目录是 $PWD, 执行拷贝..."
	(set -x; sudo cp "${OBJS[@]}" $COPY_DIR)
	(set -x; ls -alt $COPY_DIR)
else
	echo "当前目录不是 $PWD, 跳过拷贝."
fi
