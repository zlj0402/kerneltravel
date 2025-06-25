#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <poll.h>
#include <signal.h>
#include <fcntl.h>

static int fd;

static void sig_func(int sig) {

	int val;
	read(fd, &val, sizeof(val));
	printf("get button : gpio %d val %d\n", val >> 8, val & (0x0011));
}


int main(int argc, char** argv) {

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

	signal(SIGIO, sig_func);
	fcntl(fd, F_SETOWN, getpid());
	int oflags = fcntl(fd, F_GETFL);
	fcntl(fd, F_SETFL, oflags | FASYNC);
	
	while(1) {

		printf("gpio_irq_drv_read_signal -- \n");
		sleep(2);
	}

	close(fd);

	return 0;
}
