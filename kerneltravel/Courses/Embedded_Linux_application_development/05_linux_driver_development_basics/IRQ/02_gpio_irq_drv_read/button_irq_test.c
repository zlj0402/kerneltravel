#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>


int main(int argc, char** argv) {

	int fd;
	int val;

	if (argc != 2) {
		
		printf("Usage: %s <dev>\n", argv[0]);
		return -1;
	}

	fd = open(argv[1], O_RDWR);
	if (fd == -1) {

		printf("can not open file %s\n", argv[1]);
		return -1;
	}

	while(1) {

		read(fd, &val, sizeof(val));
		printf("get button : gpio %d val %d\n", val >> 8, val & (0x0011));
	}

	close(fd);

	return 0;
}
