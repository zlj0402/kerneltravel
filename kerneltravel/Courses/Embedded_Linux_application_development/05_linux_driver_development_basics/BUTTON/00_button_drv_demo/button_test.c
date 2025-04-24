#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "load.h"


/*
 * ./button_test /dev/zlj_button0
 */

int main(int argc, char* argv[])
{
	int fd;
	char val;

	/* 1. 判断参数 */
	if (argc != 2)
	{
		printf("Usage: %s <dev>\n", argv[0]);
		return -1;
	}

	/* 2. 打开文件 */
	fd = open(argv[1], O_RDWR);
	if (fd < 0)
	{
		printf("can not open fiile %s\n", argv[1]);
		return -1;
	}

	/* 3. 读文件 */
	read(fd, &val, 1);
	printf("get button: %d\n", val);

	close(fd);

	return 0;
}
