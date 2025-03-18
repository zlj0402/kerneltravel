/**
 * content: 尝试写入数据到EEPROM当中，并读取数据;
 *
 * output:
 * 			ps: 1. 代码中，设置的写入读取的地址，都是0，也就是从寄存器的首地址开始;
 *				2. 写入数据并读取：
 *					[root@100ask:/mnt/04_CodingAppOperatesHW/I2C]# ./accessing_EEPROM_at24c02 0 w liangj.zhang@qq.com
 *					[root@100ask:/mnt/04_CodingAppOperatesHW/I2C]# ./accessing_EEPROM_at24c02 0 r
 * 					get data: lliangj.zhang@qq.co
 * 					说明：我在写入的地址参数，就行了加1，所以第一个l，是之前没更改之前写入的;
 * 				3. 我又想到了i2ctransfer命令行读取试试，
 * 					[root@100ask:/mnt/04_CodingAppOperatesHW/I2C]# i2ctransfer -f -y 0 w1@0x50 0 r2
 * 					0x6c 0x6c
 * 					ps: 0x6c确实是l，且有两个;
 */
