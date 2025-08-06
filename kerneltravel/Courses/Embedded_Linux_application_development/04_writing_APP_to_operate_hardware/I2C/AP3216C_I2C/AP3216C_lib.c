#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

#include <i2cbusses.h>

#define I2C_BUS 0
#define AP3216C_ADDR 0x1e

int fd;

int ap3216c_init(int i2cbus)
{
	char filename[32];
	char buf[128];

	int err;

	struct i2c_rdwr_ioctl_data rdwr;
	int nmsgs = 1;
	struct i2c_msg msgs[nmsgs];
	int nmsgs_sent = 0;

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
	buf[0] = 0x0;	// 寄存器地址
	buf[1] = 0x4;	// 需要写入的值
	
	msgs[0].addr = AP3216C_ADDR;
	msgs[0].flags = 0;	/* 写：0，读：1 */
	msgs[0].len = 2;	// 发送两个字节（寄存器地址 + 数据）
	msgs[0].buf = buf;

	rdwr.msgs = msgs;
	rdwr.nmsgs = nmsgs;
	nmsgs_sent = ioctl(fd, I2C_RDWR, &rdwr);
	if (nmsgs_sent != nmsgs)
	{
		printf("reset AP3216C failed\n");
		return -1;
	}

	usleep(200 * 1000);	// 等待200ms

	/* enable AP3216C: write 0x3 into register 0x00 */
	buf[0] = 0x00;	// 寄存器地址
	buf[1] = 0x03;	// 需要写入的值
	
	msgs[0].addr = AP3216C_ADDR;
	msgs[0].flags = 0;
	msgs[0].len = 2;  // 发送两个字节（寄存器地址 + 数据）
	msgs[0].buf = buf;


	rdwr.msgs = msgs;
	rdwr.nmsgs = nmsgs;
	nmsgs_sent = ioctl(fd, I2C_RDWR, &rdwr);
	if (nmsgs_sent != nmsgs)
	{
		printf("enable AP3216C failed\n");
		return -1;
	}

	return 0;
}

int ap3216c_read_light(void)
{
	struct i2c_rdwr_ioctl_data rdwr;
	struct i2c_msg msgs[2];
	int nmsgs_sent;
	int nmsgs = 2;

	char buf_tx[1];
	char buf_rx[2];

	int light;

	/* 发送寄存器地址 */
	msgs[0].addr = AP3216C_ADDR;
	msgs[0].flags = 0;		/* 写：0， 读：I2C_M_RD */
	msgs[0].len = 1;
	msgs[0].buf = buf_tx;
	buf_tx[0] = 0xc;

	msgs[1].addr = AP3216C_ADDR;
	msgs[1].flags = I2C_M_RD;
	msgs[1].len = 2;
	msgs[1].buf = buf_rx;

	rdwr.msgs = msgs;
	rdwr.nmsgs = nmsgs;
	nmsgs_sent = ioctl(fd, I2C_RDWR, &rdwr);
	
	if (nmsgs_sent < 0)
	{
		fprintf(stdout, "Error: Sending messages failed: %s\n", strerror(errno));
		printf("can not read AP3216C light\n");
		return errno;
	}
	else if (nmsgs_sent < nmsgs)
	{
		fprintf(stdout, "Warning: only %d/%d messages were sent\n", nmsgs_sent, nmsgs);
		printf("can not read AP3216C light\n");
		return -1;
	}

	light = (buf_rx[1] << 8) | buf_rx[0];

	return light;
}

int ap3216c_read_distance(void)
{
	struct i2c_rdwr_ioctl_data rdwr;
	struct i2c_msg msgs[2];
	int nmsgs_sent;
	int nmsgs = 2;

	char buf_tx[1];
	char buf_rx[2];

	int distance;

	/* 发送寄存器地址 */
	msgs[0].addr = AP3216C_ADDR;
	msgs[0].flags = 0;		/* 写：0， 读：I2C_M_RD */
	msgs[0].len = 1;
	msgs[0].buf = buf_tx;
	buf_tx[0] = 0xe;

	msgs[1].addr = AP3216C_ADDR;
	msgs[1].flags = I2C_M_RD;
	msgs[1].len = 2;
	msgs[1].buf = buf_rx;

	rdwr.msgs = msgs;
	rdwr.nmsgs = nmsgs;
	nmsgs_sent = ioctl(fd, I2C_RDWR, &rdwr);
	if (nmsgs_sent < 0)
	{
		fprintf(stdout, "Error: Sending messages failed: %s\n", strerror(errno));
		printf("can not read AP3216C distance\n");
		return errno;
	}
	else if (nmsgs_sent < nmsgs)
	{
		fprintf(stdout, "Warning: only %d/%d messages were sent\n", nmsgs_sent, nmsgs);
		printf("can not read AP3216C distance\n");
		return -1;
	}

	distance = ((buf_rx[1] & 0x3f) << 4) | (buf_rx[0] & 0xf);

	return distance;
}
