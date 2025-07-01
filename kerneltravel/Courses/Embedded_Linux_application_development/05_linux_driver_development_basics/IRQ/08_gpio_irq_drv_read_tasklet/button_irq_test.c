#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>


int main(int argc, char** argv) {

	int fd;
	int val;
	int ret;
	int i;

	if (argc != 2) {
		
		printf("Usage: %s <dev>\n", argv[0]);
		return -1;
	}

	fd = open(argv[1], O_RDWR | O_NONBLOCK);
	if (fd == -1) {

		printf("can not open file %s\n", argv[1]);
		return -1;
	}

	for (i = 0; i < 10; ++i) {

		if (read(fd, &val, sizeof(int)) == sizeof(int))
			printf("for-loop get button : gpio %d val %d\n", val >> 8, val & (0x0011));
		else
			printf("get button -1\n");
	}

	int oflags = fcntl(fd, F_GETFL);
	fcntl(fd, F_SETFL, oflags & (~O_NONBLOCK));
	
	while(1) {

		if (read(fd, &val, sizeof(int) == sizeof(int)))
			printf("get button : gpio %d val %d\n", val >> 8, val & (0x0011));
		else
			printf("while get button -1\n");
	}

	close(fd);

	return 0;
}
