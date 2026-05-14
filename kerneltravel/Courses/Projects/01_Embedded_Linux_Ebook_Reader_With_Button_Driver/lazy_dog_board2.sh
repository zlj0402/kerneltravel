#!/bin/bash
set -x

echo "7 4 1 7" > /proc/sys/kernel/printk

echo 0 > /sys/class/graphics/fb0/blank

# -------------

rmmod gpio_irq_drv.ko
insmod gpio_irq_drv.ko

# --------------

rmmod button.ko
insmod button.ko

# --------------

rmmod button_platform.ko
rmmod button_chrdev.ko
rmmod button_core.ko

insmod button_core.ko
insmod button_chrdev.ko
insmod button_platform.ko

./show_file -l -s 24 -f STKAITI.TTF -h HZK16 utf8_novel.txt
