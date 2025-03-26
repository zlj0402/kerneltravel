#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "load.h"

/* ./hello_test r
 */

int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		printf("Usage:\n");
		printf("%s r		  - read string from hello_drv.\n", argv[0]);
		return -1;
	}

	/* open */
	int fd = open("/dev/abcxyz", O_RDWR);
	if (fd < 0)
	{
		printf("open /dev/abcxyz error\n");
		return -1;
	}

	int len;
	char buf[32];

	if (argc == 2 && (argv[1][0] == 'r'))
	{
		/* read */
		len = read(fd, buf, sizeof(buf));
		buf[len - 1] = '\0';
		
		/* printf */
		printf("len: %d, strlen: %d, get str from drv: %s\n", len, strlen(buf) + 1, buf);
	}

	// 不主动关闭，系统也会自动调用close，close调用到release
	// close(fd);

	return 0;
}
