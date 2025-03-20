#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

#include <i2cbusses.h>
#include <i2c/smbus.h>

#define I2C_BUS 0
#define AP3216C_ADDR 0x1e

int fd;

int ap3216c_init(int i2cbus)
{
	char filename[32];
	char buf[128];

	int err;

	// int open_i2c_dev(int i2cbus, char *filename, size_t size, int quiet)
	fd = open_i2c_dev(i2cbus, filename, sizeof(filename), 0);
	if (fd < 0)
	{
		printf("can not open %s\n", filename);
		return -1;
	}

	// set_slave_addr(int file, int address, int force)
	err = set_slave_addr(fd, AP3216C_ADDR, 1);
	// err == 0 => all right; or error happened
	if (err)
	{
		printf("set_slave_addr %x error\n", AP3216C_ADDR);
		return err;
	}

	/* reset AP3216C: write 0x4 into register 0x00 */
	//__s32 i2c_smbus_write_word_data(int file, __u8 command, __u16 value)
	int command = 0x00;
	int value = 0x04;
	err = i2c_smbus_write_byte_data(fd, command, value);
	if (err < 0)
	{
		printf("reset AP3216C failed\n");
		return -1;
	} else {
		printf("reset AP3216C, got data: %d\n", err);
	}

	usleep(200 * 1000);	// 等待200ms

	/* enable AP3216C: write 0x3 into register 0x00 */
	value = 0x03;
	err = i2c_smbus_write_byte_data(fd, command, value);
	
	if (err < 0)
	{
		printf("enable AP3216C failed\n");
		return -1;
	} else {
		printf("enable AP3216C, got data: %d\n", err);
	}

	return 0;
}

int ap3216c_read_light(void)
{
	int light;
	// __s32 i2c_smbus_read_word_data(int file, __u8 command)
	int command = 0x0c;
	int err = i2c_smbus_read_word_data(fd, command);
	
	if (err < 0)
	{
		printf("can not read AP3216C light\n");
		return errno;
	}
	else
	{
		printf("read AP3216C light, got data: %d\n", err);
		light = err;
	}

	return light;
}

int ap3216c_read_distance(void)
{
	int distance;
	// __s32 i2c_smbus_read_word_data(int file, __u8 command)
	int command = 0x0e;
	int err = i2c_smbus_read_word_data(fd, command);

	char buf[2];
	
	if (err < 0)
	{
		printf("can not read AP3216C distance\n");
		return errno;
	}
	else
	{
		printf("read AP3216C distance, got data: %d\n", err);
		buf[0] = err & 0xff;	// 低字节
		buf[1] = err >> 8;		// 高字节
		distance = ((buf[1] & 0x3f) << 4) | (buf[0] & 0xf);
	}
	//  ((buf_rx[1] & 0x3f) << 4) | (buf_rx[0] & 0xf); -> I2C版本的组装方式

	return distance;
}
