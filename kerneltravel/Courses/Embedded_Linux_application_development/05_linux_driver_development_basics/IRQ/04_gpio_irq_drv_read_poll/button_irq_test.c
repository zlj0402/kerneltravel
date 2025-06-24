#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <poll.h>


int main(int argc, char** argv) {

	int fd;
	int val;
	int nfds = 1;
	struct pollfd fds[nfds];
	int timeout_ms = 5000;
	int ret;

	if (argc != 2) {
		
		printf("Usage: %s <dev>\n", argv[0]);
		return -1;
	}

	fd = open(argv[1], O_RDWR);
	if (fd == -1) {

		printf("can not open file %s\n", argv[1]);
		return -1;
	}

	fds[0].fd = fd;
	fds[0].events = POLLIN;
	
	while(1) {

		ret = poll(fds, nfds, timeout_ms);
		if (ret == 1 && (fds[0].revents & POLLIN)) {
			
			read(fd, &val, sizeof(val));
			printf("get button : gpio %d val %d\n", val >> 8, val & (0x0011));
		}
		else {
			printf("timeout\n");
		}
	}

	close(fd);

	return 0;
}
