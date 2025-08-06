#include <stdio.h>
#include <i2c/smbus.h>
#include <time.h>
#include "i2cbusses.h"

#include "load.h"

/**
 * Usage: ./at24c02_test SMBUS_NO. w "zlj0402@git.com"
 * 		  ./at24c02_test SUBUS_NO. r
 */

int main(int argc, char* argv[])
{
	unsigned int dev_addr = 0x50;
	unsigned int mem_addr = 0;
	unsigned char buf[128];
	
	unsigned char* str;
	int fd;
	char filename[32];

	struct timespec sleep_time;
	sleep_time.tv_sec = 0;
	sleep_time.tv_nsec = 20000000;

	if (!(argc == 3 || argc == 4))
	{
		printf("Usage:\n");
		printf("write eeprom: %s SMBUS_NO. w <string>\n", argv[0]);
		printf("read  eeprom: %s SUBUS_NO. r\n", argv[0]);
		return -1;
	}

	// int open_i2c_dev(int i2cbus, char *filename, size_t size, int quiet) 
	fd = open_i2c_dev(argv[1][0] - '0', filename, sizeof(filename), 0);
	if (fd < 0)
	{
		printf("can not open %s\n", filename);
		return -1;
	}

	// int set_slave_addr(int file, int address, int force)
	int res = set_slave_addr(fd, dev_addr, 1);
	if (res)
	{
		printf("res = %d\n", res);
		printf("can not set_slave_addr\n");
		return -1;
	}

	if (argv[2][0] == 'w')
	{
		// write str: argv[3]
		str = argv[3];
		while (*str)
		{
			// __s32 i2c_smbus_write_byte_data(int file, __u8 command, __u8 value)
			// __u8 command -> 寄存器的存储地址
			int ret = i2c_smbus_write_byte_data(fd, mem_addr + 1, *str);
			if (ret < 0)
			{
				printf("i2c_smbus_write_byte_data error\n");
				return -1;
			}
			nanosleep(&sleep_time, NULL);
			mem_addr++;
			str++;
		}
		// 写入字符串结束符
		i2c_smbus_write_byte_data(fd, mem_addr, 0);
	}
	else // read
	{
		// 不用SMBus Block Read，是因为EEPROM不会上报Block Count
		// 这里用I2C Block Read，因为这个封装函数，没有从设备上传的Block Count
		// __s32 i2c_smbus_read_i2c_block_data(int file, __u8 command, __u8 length, const __u8 *values)
		int read_cnt = i2c_smbus_read_i2c_block_data(fd, mem_addr, sizeof(buf), buf);
		if (read_cnt < 0)
		{
			printf("i2c_smbus_read_i2c_block_data error\n");
			return -1;
		}
		buf[read_cnt] = '\0';

		printf("get data: %s\n", buf);
	}
	return 0;
}
