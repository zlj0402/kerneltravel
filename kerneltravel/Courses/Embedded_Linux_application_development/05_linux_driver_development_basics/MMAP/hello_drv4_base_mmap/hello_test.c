#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>


#include "load.h"

/* ./hello_test
 */

int main(int argc, char* argv[])
{

	int len;
	char *buf;
	char str[1024];

	/* open */
	int fd = open("/dev/abcxyz", O_RDWR);
	if (fd < 0)
	{
		printf("open /dev/abcxyz error\n");
		return -1;
	}

	/* mmap */
 	//buf = mmap(NULL, 8 * 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
 	buf = mmap(NULL, 8 * 1024, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (buf == MAP_FAILED) {
		printf("can not mmap\n");
		return -1;
	}

	printf("mmap address: %p\n", buf);
	printf("buf origin data: %s\n", buf);	// 应该是 old
	
	/* write */
	strcpy(buf, "new");	// buf 内容确实是新的，但是 buf 已经绑定到 copy 的那份地址了;

	/* read & compare */
	read(fd, str, sizeof(str));		// 应该还是 old

	printf("buf new data: %s\n", buf);

	if (strcmp(buf, str) == 0) {
		printf("compare ok!\n");
	} 
	else {
		printf("compare err!\n");
		printf("str read: %s\n", str);
	}

	while(1) {
		sleep(10);	// 停顿应用程序，cat /proc/pid/maps 查看 pid 进程的内存映射情况
	}

	munmap(buf, 8 * 1024);

	// 不主动关闭，系统也会自动调用close，close调用到release
	// close(fd);

	return 0;
}
