// printf / perror
#include <stdio.h>
// open、fstat
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// struct fb_var_resolution
#include <linux/fb.h>
// ioctl
#include <sys/ioctl.h>
// close、fstat
#include <unistd.h>
// mmap
#include <sys/mman.h>
// memset
#include <string.h>
// random
#include <stdlib.h>
// time
#include <time.h>
// fstat
#include <sys/stat.h>


#include "fontdata_8x16.h"
#include "load.h"

#define FBDEV "/dev/fb0"
#define MAXRGB 0xffffff
#define BGRGB 0xffffff
#define HZK16 "HZK16"
#define TIMESTRLEN 64

static int fb_fd;
static int hzk_fd;
static struct fb_var_screeninfo info;
static int bpp_st;	// bits_per_pixel size_t
static int line_st;
static int screen_st;
static unsigned char* fb_base;
static unsigned char* hzk_base;

static char time_str[64];

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

unsigned int random_rgb888(void) {

	return random() % (MAXRGB - 0 + 1);
}

unsigned int random_Chinese_gb2312(void) {

	unsigned int first_byte = random() % (0xf7 - 0xa1 + 1) + 0xa1;
	unsigned int sec_byte = random() % (0xfe - 0xa1 + 1) + 0xa1;

	return (first_byte << 8) | sec_byte;
}

void lcd_put_ascii(int x, int y, char c, unsigned int color) {
	
	if (x >= info.xres || y >= info.yres) {

		printf("(%d, %d) out of screen size\n", x, y);
		return;
	}

	unsigned char* char_base = (unsigned char*)fontdata_8x16 + c * 16;

	for (int i = 0; i < 16; ++i) {

		unsigned char cv = *(char_base + i);

		int cy = y + i;
		for (int j = 0; j < 8; ++j) {

			int cx = x + j;
			int cval = (cv >> (8 - j - 1)) & 0x01;
			if (cx < info.xres && cy < info.yres)
				lcd_put_pixel(cx, cy, cval ? color : BGRGB);
		}
	}
}

void lcd_put_Chinese(int x, int y, unsigned int hz, unsigned int color) {

	if (x >= info.xres || y >= info.yres) {

		printf("(%d, %d) out of screen size\n", x, y);
		return;
	}

	int area = (hz >> 8) - 0xa1;
	int where = (hz & 0xff) - 0xa1;

	unsigned char* char_base = hzk_base + (area * 94 + where) * 32;	// * 32 -> per Chinese char takes 32 bytes

	for (int i = 0; i < 16; ++i) {

		for (int j = 0; j < 2; ++j) {

			unsigned char cv = *(char_base + 2 * i + j);
			int cy = y + i;
			for (int k = 0; k < 8; ++k) {

				int cx = x + j * 8 + k;
				int cval = (cv >> (8 - k - 1)) & 0x01;
				if (cx < info.xres && cy < info.yres)
					lcd_put_pixel(cx, cy, cval ? color : BGRGB);
			}
		}
	}
}

// should called after getting screen info
void draw_an_ascii(void) {

	char char_v = random() % 26 + (random() % 2 ? 'a' : 'A');
	unsigned int char_color = random_rgb888();
	unsigned int char_x = random() % info.xres;
	unsigned int char_y = random() % info.yres;
	
	printf("ASCII: %c, start coordinates: (%d, %d), rgb: 0x%x\n", char_v, char_x, char_y, char_color);
	lcd_put_ascii(char_x, char_y, char_v, char_color);
}

// should called after opening HZK16(GBK2312)
void draw_a_Chinese(void) {

	unsigned int hz = random_Chinese_gb2312();
	unsigned int char_color = random_rgb888();
	unsigned int char_x = random() % info.xres;
	unsigned int char_y = random() % info.yres;

	printf("HZ: 0x%x, start coordinates: (%d, %d), rgb: 0x%x\n", hz, char_x, char_y, char_color);
	lcd_put_Chinese(char_x, char_y, hz, char_color);
}

char* format_time_custom(time_t timestamp, char* buf, int buf_len) {

    struct tm *tm = localtime(&timestamp); // 转换为本地时间结构体
    if (!tm) return NULL;

    // 格式化为 "2025/7/16 10:03"
    strftime(buf, buf_len, "%Y/%-m/%-d %-H:%M", tm);
    return buf;
}


int main(int argc, char** argv) {
	
	if (argc != 1) {

		printf("Usage: %s\n", argv[0]);
		return -1;
	}

	srand(time(NULL));

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
	
	printf("Resolution(xres * yres): %d x %d, %d bits per pixel.\n", info.xres, info.yres, info.bits_per_pixel);

	bpp_st = info.bits_per_pixel / 8;
	line_st = info.xres * bpp_st;
	screen_st = info.xres * info.yres * bpp_st;
	printf("one frame takes %d bytes == %d KB == %.3g MB.\n\n", screen_st, screen_st / 1024, screen_st / 1024 / 1024.0);

	// 3. mmap 来获取 Framebuffer 在内核中映射地址（返回的是虚拟地址），我们来直接读写;
 	fb_base = mmap(NULL, screen_st, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
	if ((void*)fb_base == MAP_FAILED) {	// MAP_FAILED -> (void*)-1
		
		perror("Framebuffer mmap error happened.\n");
		close(fb_fd);
		return -1;
	}

	printf("fb_base: %p\n", (void*)fb_base);

	// 3.2 打开 HZK16 文件，映射这个文件地址
	hzk_fd = open(HZK16, O_RDONLY);
	if (hzk_fd < 0) {

		printf("can not open %s\n", HZK16);
		close(fb_fd);
		munmap(fb_base, screen_st);
		return -1;
	}

	struct stat statbuf;
	if (fstat(hzk_fd, &statbuf) == -1) {
		
		printf("can not get %s info\n", HZK16);
		close(fb_fd);
		close(hzk_fd);
		munmap(fb_base, screen_st);
		return -1;
	}
	hzk_base = mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED, hzk_fd, 0);
	if ((void*)hzk_base == MAP_FAILED) {
		
		printf("%s mmap error happened.\n", HZK16);
		close(fb_fd);
		close(hzk_fd);
		munmap(fb_base, screen_st);
		return -1;
	}

	printf("hzk_base: %p\n", (void*)hzk_base);
	printf("%s's size: %d bytes (%.3g KB), last access time: %s, last modified time: %s\n", 
		HZK16, statbuf.st_size, statbuf.st_size / 1024.0, 
		format_time_custom(statbuf.st_atime, time_str, TIMESTRLEN),
		format_time_custom(statbuf.st_mtime, time_str, TIMESTRLEN));
	
	// 4. draw

	// clear screen
	clear_screen(BGRGB);

	// 从屏幕中心，画一条线
	for (int i = 0; i < 100; ++i)
		lcd_put_pixel(info.xres / 2 + i, info.yres / 2, 0xeebbff);

	// 显示一个 ASCII 码
	draw_an_ascii();

	// 显示一个 GB2312 中的字符
	draw_a_Chinese();

	munmap(fb_base, screen_st);
	close(fb_fd);

	return 0;
}
