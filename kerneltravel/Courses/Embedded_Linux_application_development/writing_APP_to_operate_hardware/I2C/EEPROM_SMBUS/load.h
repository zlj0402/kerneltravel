/**
 * content: 尝试写入数据到EEPROM当中，并读取数据;
 *
 * output:
 * 			ps: 1. 代码中，设置的写入读取的地址，都是0，也就是从寄存器的首地址开始;
 *				2. 写入数据并读取：
 *					[root@100ask:/mnt/04_CodingAppOperatesHW/I2C]# ./accessing_EEPROM_at24c02 0 w liangj.zhang@qq.com
 *					[root@100ask:/mnt/04_CodingAppOperatesHW/I2C]# ./accessing_EEPROM_at24c02 0 r
 * 					get data: lliangj.zhang@qq.co
 * 					说明：我在写入的地址参数的地方，进行了加1，所以第一个l，是之前没更改之前写入的;
 * 					正常的从地址0x1的地方开始写入的话，即使是采用参数的地方加1的方式，最后的字符串空白符的写入，也是应该在mem_addr+1的地方写入，这样在后面进行读取的时候，应该是lliangj.zhang@qq.com
 * 				3. 我又想到了i2ctransfer命令行读取试试，
 * 					[root@100ask:/mnt/04_CodingAppOperatesHW/I2C]# i2ctransfer -f -y 0 w1@0x50 0 r2
 * 					0x6c 0x6c
 * 					ps: 0x6c确实是l，且有两个;
 * 
 * description:
 * 			> 为什么要加上一个休眠时间（nanosleep 20ms）
 * 				EEPROM手册，表示在接受到一个数据之后，EEPROM会进入一个内部的写循环，需要t_wr的时间（10ms）, 这段时间的写入都是禁止的；
 *
 * 			> 尝试了给开发板断电，再重启，再次执行：i2ctransfer -f -y 0 w1@0x50 0 r2
 * 				output:
 * 					[root@100ask:/mnt/04_CodingAppOperatesHW/I2C]# i2ctransfer -f -y 0 w1@0x50 0 r2
 * 					0x6c 0x6c
 * 					+ 可以看出来，EEPROM断电之后，数据确实能够保存下来，还是ll的ascii码；
 */
