// printf / perror
#include <stdio.h>
// open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// struct fb_var_resolution
#include <linux/fb.h>
// ioctl
#include <sys/ioctl.h>
// close
#include <unistd.h>

#include "load.h"

#define FBDEV "/dev/fb0"


static int fb_fd;
struct fb_var_screeninfo info;

int main(int argc, char** argv) {

	int err;
	
	if (argc != 1) {

		printf("Usage: %s\n", argv[0]);
		return -1;
	}

	// 打开帧缓冲设备
	fb_fd = open(FBDEV, O_RDWR);
	
	if (fb_fd < 0) {
		
		printf("can not open %s\n", FBDEV);
		return -1;
	}
	
	printf("Framebuffer device: %s opened successfully!\n", FBDEV);

	// Usually,  on  success  zero is returned.
	if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &info)) {

		perror("Error reading screen resolution variable information.\n");
		close(fb_fd);
		return -1;
	}

	printf("x_res: %d, y_res = %d\n", info.xres, info.yres);
	printf("bits_per_pixel = %d\n", info.bits_per_pixel);

	close(fb_fd);

	return 0;
}
