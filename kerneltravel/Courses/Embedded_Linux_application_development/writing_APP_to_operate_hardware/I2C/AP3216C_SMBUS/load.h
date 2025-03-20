/**
 * @brief 读AP3216C的光感值（0xc、0xd）和距离值（0xe、0xf） -- SMBUS 的方式
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
 *
 * @ output:
 *			[root@100ask:/mnt/04b_CodingAppOperatesHW/27_smbus_ap3216c]# ./AP3216C_test 0 -d
 *			get i2cbus: 0
 *			reset AP3216C, got data: 0
 *			enable AP3216C, got data: 0
 *			read AP3216C distance, got data: 0
 *			read distance: 0, 0x0
 *			read AP3216C distance, got data: 42117
 *			read distance: 581, 0x245
 *			...
 *
 *			[root@100ask:/mnt/04b_CodingAppOperatesHW/27_smbus_ap3216c]# ./AP3216C_test 0 -l
 *			get i2cbus: 0
 *			reset AP3216C, got data: 0
 *			enable AP3216C, got data: 0
 *			read AP3216C light, got data: 0
 *			read light: 0 0x0
 *			read AP3216C light, got data: 52
 *			read light: 52 0x34
 *			read AP3216C light, got data: 53
 *			read light: 53 0x35
 *			...
 *
 *			ps:
 *				+ i2c_smbus_write_byte_data的返回值，成功写入时，return 0;
 *				+ i2c_smbus_read_word_data的放回值，成功写入时，return 读取的数值;
 *					+ 至于读取distance时，对读到的数值是处理过的；AP3216C的手册上，给出了表示distance的对应位，高位字节前6位，低位字节前4位，一起表示distance;
 *					+ light值，就是两个字节，直接返回就OK;
 */
