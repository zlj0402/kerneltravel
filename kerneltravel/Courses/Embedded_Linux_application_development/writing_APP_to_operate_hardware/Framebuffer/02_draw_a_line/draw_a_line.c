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
// mmap
#include <sys/mman.h>
// memset
#include <string.h>

#include "load.h"

#define FBDEV "/dev/fb0"


static int fb_fd;
static struct fb_var_screeninfo info;
static int bpp_st;	// bits_per_pixel size_t
static int line_st;
static int screen_st;
static unsigned char* fb_base;

void clear_screen(unsigned int color) {
	memset(fb_base, color & 0xff, screen_st);	// memset 一字节一字节填充，不 &0xff，memset 取的也是低 8 位
}

// @param color: 32 位的颜色值: RGB888 格式
void lcd_put_pixel(int x, int y, unsigned int color) {

	if (x >= info.xres || y >= info.yres) {

		printf("(%d, %d) out of screen size\n", x, y);
		return;
	}

	unsigned char* 	pen_8 = fb_base + y * line_st + x * bpp_st;
	unsigned short* pen_16;
	unsigned int* 	pen_32;

	unsigned short red, green, blue;

	pen_16 = (unsigned short*) pen_8;
	pen_32 = (unsigned int*) pen_8;

	switch(info.bits_per_pixel) {

		case 8:
		{	// 对于 8bpp，color 就不再表示 RBG 三原色了，这涉及调色板的概念，color 是调色板的值。
			*pen_8 = color;
			break;
		}
		case 16:
		{	// 把 red、green、blue 这三种 8 位颜色值，根据 RGB565 的格式，
			// 只保留 red 中的高 5 位、green 中的高 6 位、blue 中的高 5 位，组合成一个新的 16 位颜色值。
			red = (color >> 16) & 0xff;
			green = (color >> 8) & 0xff;
			blue = (color) & 0xff;

			color = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
			*pen_16 = color;
			break;
		}
		case 32:
		{	// 对于 32bpp，颜色格式跟 color 参数一致，可以直接写入 Framebuffer。
			*pen_32 = color;
			break;
		}
		default:
		{
			printf("can not support %d bpp\n", info.bits_per_pixel);
			break;
		}
	}
}

int main(int argc, char** argv) {
	
	if (argc != 1) {

		printf("Usage: %s\n", argv[0]);
		return -1;
	}

	// 1. 打开帧缓冲设备 -- open
	fb_fd = open(FBDEV, O_RDWR);
	
	if (fb_fd < 0) {
		
		printf("can not open %s\n", FBDEV);
		return -1;
	}
	
	printf("Framebuffer device: %s opened successfully!\n", FBDEV);

	// 2. 得到屏幕信息 -- ioctl
	// Usually,  on  success  zero is returned.
	if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &info)) {

		perror("Error reading screen resolution variable information.\n");
		close(fb_fd);
		return -1;
	}

	printf("x_res: %d, y_res = %d\n", info.xres, info.yres);
	printf("bits_per_pixel = %d\n", info.bits_per_pixel);

	bpp_st = info.bits_per_pixel / 8;
	line_st = info.xres * bpp_st;
	screen_st = info.xres * info.yres * bpp_st;
	printf("one frame takes %d bytes == %d KB == %.3g MB.\n\n", screen_st, screen_st / 1024, screen_st / 1024 / 1024.0);

	// 3. mmap 来获取 Framebuffer 在内核中映射地址（返回的是虚拟地址），我们来直接读写;
 	fb_base = mmap(NULL, screen_st, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
	if ((void*)fb_base == MAP_FAILED) {	// MAP_FAILED -> (void*)-1
		
		perror("mmap error happened.\n");
		close(fb_fd);
	}

	printf("fb_base: %p\n", (void*)fb_base);

	// 4. draw

	// clear screen
	clear_screen(0xff);

	// 从屏幕中心，画一条线
	for (int i = 0; i < 100; ++i)
		lcd_put_pixel(info.xres / 2 + i, info.yres / 2, 0xeebbff);

	munmap(fb_base, screen_st);
	close(fb_fd);

	return 0;
}
