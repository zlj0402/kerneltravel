/**
 * @brief 读AP3216C的光感值（0xc、0xd）和距离值（0xe、0xf）
 *
 * @ compile & execute:
 * 			1. 在ubuntu虚拟机上，设置工具链的环境变量：
 * 				export ARCH=arm
 * 				export BUILDROOT_CROSS_COMPILE=arm-buildroot-linux-gnueabihf-
 * 				export CC_PATH=/home/zlj/Codes/100ask_imx6ull-sdk/ToolChain/gcc-linaro-6.2.1-2016.11-x86_64_arm-linux-gnueabihf/bin
 * 				export PATH=$PATH:$(CC_PATH)
 * 				----------------------------
 * 				ps: 我的Makefile中，都是这么使用工具链的：$(BUILDROOT_CROSS_COMPILE)gcc
 * 			2. 程序所在目录下，执行：make
 * 			3. 将生成的可执行文件，从ubuntu虚拟机拷贝到开发板和ubuntu虚拟机共享的目录下;
 * 			4. ./AP3216C_test <i2cbus_no.> -l/d
 * 				example:
 * 					read light: ./AP3216C_test 0 -l
 * 					read distance: ./AP3216C_test 0 -d
 * 					read value of light/distance per 3 seconds
 */
