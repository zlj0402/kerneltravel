#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "load.h"

/* ./hello_test r
 * ./hello_test w weidongshan
 */

int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		printf("Usage:\n");
		printf("%s r		  - read string from hello_drv.\n", argv[0]);
		printf("%s w <string> - write string to hello_drv.\n", argv[0]);
		return -1;
	}

	char status;
	/* 1. open */
	int fd = open(argv[1], O_RDWR);
	if (fd < 0)
	{
		printf("open %s error\n", argv[1]);
		return -1;
	}

	int len;
	char buf[32];

	/* 3. write file */
	if (0 == strcmp(argv[2], "on"))
	{
		status = 1;
		write(fd, &status, 1);
	}
	else
	{
		status = 0;
		write(fd, &status, 1);
	}

	// 不主动关闭，系统也会自动调用close，close调用到release
	// close(fd);

	return 0;
}
