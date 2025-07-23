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
// wchar_t
#include <wchar.h>
// cos、sin
#include <math.h>
// freetype
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_FONT_FORMATS_H 

#include "load.h"

#define FBDEV "/dev/fb0"
#define MAXRGB 0xffffff
#define BGRGB 0xffffff
#define TIMESTRLEN 64
#define TTFNAME "STKAITI.TTF"

static int fb_fd;
static struct fb_var_screeninfo info;
static int bpp_st;	// bits_per_pixel size_t
static int line_st;
static int screen_st;
static unsigned char* fb_base;

static char time_str[64];

// ------------------------- FT variables -------------------------

static FT_Library library;
static FT_Error error;
static FT_Face face;
static FT_GlyphSlot  slot;

static int ttf_fd;
static unsigned char* ttf_base;
static struct stat ttf_stat;

#define LENGTH 300
#define HEIGHT 200

// --------------------------- Utils -------------------------

void clear_screen(unsigned int color) {
	memset(fb_base, color & 0xff, screen_st);	// memset 一字节一字节填充，不 &0xff，memset 取的也是低 8 位
}

unsigned int random_rgb888(void) {

	return random() % (MAXRGB - 0 + 1);
}

// 常用简体/繁体汉字范围：0x4E00 ~ 0x9FFF
unsigned int random_Chinese_unicode(void) {

	return random() % (0x9FFF - 0x4E00 + 1) + 0x4E00;
}

char* format_time_custom(time_t timestamp, char* buf, int buf_len) {

    struct tm *tm = localtime(&timestamp); // 转换为本地时间结构体
    if (!tm) return NULL;

    // 格式化为 "2025/7/16 10:03"
    strftime(buf, buf_len, "%Y/%-m/%-d %-H:%M", tm);
    return buf;
}

// --------------------------- Function Utils -------------------------

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

// ------------------------------------- draw -----------------------------------------

// [x, y] -> top_left
// (x_max, y_max) -> bottom_right
void lcd_put_Chinese_ft(FT_Bitmap* bitmap, FT_Int x, FT_Int y, unsigned int hz_uni, unsigned int color) {

	if (x >= info.xres || y >= info.yres) {

		printf("(%d, %d) out of screen size\n", x, y);
		return;
	}

	printf("%s x = %d, y = %d\n", __func__, x, y);
	
	for (int i = 0; i < bitmap->rows; ++i) {

		int cy = y + i;
		for (int j = 0; j < bitmap->width; ++j) {

			int cx = x + j;
			if (cx >= info.xres || cy >= info.yres)
				continue;
			//printf("%d ", bitmap->buffer[i * bitmap->width + j]);
			lcd_put_pixel(cx, cy, bitmap->buffer[i * bitmap->width + j] ? color : BGRGB);
		}
		//printf("\n");
	}
}

void draw_Chinese_line(wchar_t* str) {

	unsigned int char_color = random_rgb888();
	unsigned int char_x = LENGTH;
	unsigned int char_y = HEIGHT;


	printf("str len: %d\n", wcslen(str));
	for (int i = 0; i < wcslen(str); ++i) {

		unsigned int wchar = (unsigned int)str[i];
		
		printf("UNI: 0x%x, start coordinates: (%d, %d), rgb: 0x%x\n", wchar, char_x, char_y, char_color);
	
		/* 加载 glyph 图像到 slot（会清除之前的 glyph） */
		if (FT_Load_Char(face, wchar, FT_LOAD_RENDER)) {
			printf("Failed to load character 0x%x!\n", wchar);
		}
		slot = face->glyph;	// 原来缺少这行，&slot->bitmap 就会报 segment fault，因为 slot 是空的;

		lcd_put_Chinese_ft(&slot->bitmap, char_x, char_y, wchar, char_color);

		char_x += slot->advance.x >> 6;
		char_y += slot->advance.y >> 6;
	}
}

void draw_Chinese_line_with_pen(wchar_t* str) {

    unsigned int char_color = random_rgb888();
	
	double angle		  = ( 180.0 / 360 ) * 3.14159 * 2;	   /* use 25 degrees	 */

    FT_Matrix matrix;
	matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
	matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
	matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
	matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );


    // 起始绘制位置（注意：单位是 1/64 像素，FreeType 用 fixed point）
    FT_Vector pen;
    pen.x = LENGTH * 64;                     // X: 从屏幕左侧 LENGTH 像素位置开始
    pen.y = (info.yres - HEIGHT) * 64;       // Y: 屏幕高度 - HEIGHT，坐标系向下

    size_t len = wcslen(str);
    printf("str len: %zu\n", len);

    for (size_t i = 0; i < len; ++i) {
        unsigned int wchar = (unsigned int)str[i];
        printf("UNI: 0x%x, pen: (%ld, %ld), rgb: 0x%x\n", wchar, pen.x, pen.y, char_color);

        // 设置变换（包括位置）
        FT_Set_Transform(face, &matrix, &pen);

        // 加载 glyph 图像
        if (FT_Load_Char(face, wchar, FT_LOAD_RENDER)) {
            printf("Failed to load character 0x%x!\n", wchar);
            continue;
        }

        slot = face->glyph;

        // 注意：slot->bitmap_top 是相对 pen.y 的偏移（FreeType 坐标）
        int draw_x = slot->bitmap_left;
        int draw_y = info.yres - slot->bitmap_top;  // 从顶部向下绘制（像素坐标）

        lcd_put_Chinese_ft(&slot->bitmap, draw_x, draw_y, wchar, char_color);

        // 更新 pen 位置
        pen.x += slot->advance.x;
        pen.y += slot->advance.y;
    }
}


// ----------------------------- initialize & close -----------------------------------

int init_ft_library(void) {

	printf("\n%s\n", __func__);
	error = FT_Init_FreeType(&library);
	if (error) {

		printf("Failed to initialize FreeType library: error code %d\n", error);
		return -1;
	}

	// 1. open ttf file
	ttf_fd = open(TTFNAME, O_RDONLY);
	if (ttf_fd < 0) {

		printf("open %s error\n", TTFNAME);
  		FT_Done_FreeType( library );
		return -1;
	}

	// 2. get ttf file info
	if (fstat(ttf_fd, &ttf_stat) == -1) {
		
		printf("can not get %s info\n", TTFNAME);
  		FT_Done_FreeType( library );
		return -1;
	}

	ttf_base = mmap(0, ttf_stat.st_size, PROT_READ, MAP_SHARED, ttf_fd, 0);
	if (ttf_base == (unsigned char*)MAP_FAILED) {

		close(ttf_fd);
  		FT_Done_FreeType( library );
		return -1;
	}

	// 3. get face, using ttf file info
	error = FT_New_Memory_Face(library, ttf_base, ttf_stat.st_size, 0, &face);
	if (error) {
		
        fprintf(stderr, "Failed to load font from memory: %d\n", error);
  		FT_Done_FreeType( library );
		return -1;
    }

	printf("Number of glyphs: %ld\n", face->num_glyphs);

	// 3.2 print face info
	if (face->face_flags & FT_FACE_FLAG_SCALABLE) {
	    printf("This is a scalable font.\n");
	    printf("Units per EM: %d\n", face->units_per_EM);
	}

	printf("Number of fixed sizes: %d\n", face->num_fixed_sizes);
	const char* font_format = FT_Get_Font_Format(face);
	printf("Font format: %s\n", font_format);
	for (int i = 0; i < face->num_fixed_sizes; i++) {
	    FT_Bitmap_Size size = face->available_sizes[i];
	    printf("Strike %d: height=%d, width=%d, size=%ld, x_ppem=%d, y_ppem=%d\n",
	           i, size.height, size.width, size.size, size.x_ppem, size.y_ppem);
	}
	if (face->num_fixed_sizes == 0) {

		printf("vector char doesn't need fixed size\n");
	}

	// 3.3 set size
	int width = 64;
	while (error = FT_Set_Pixel_Sizes(face, width, 0)) {
			
		fprintf(stderr, "Failed to set pixel sizes: %d\n", error);
		width += 1;
		if (width > 600)
			return -1;
	}
	printf("x_ppem: %d\n", face->size->metrics.x_ppem);
	printf("y_ppem: %d\n", face->size->metrics.y_ppem);

	printf("\n");
	return 0;
}

void close_library() {
	
 	FT_Done_Face    ( face );
  	FT_Done_FreeType( library );
	munmap(ttf_base, ttf_stat.st_size);
	close(ttf_fd);
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

	// 4. 初始化 FT_LIBRARY & 打开 TTF 字体文件 & print 字体信息
	if (init_ft_library() == -1)
	{
		printf("init_ft_library error\n");
		return -1;
	}
	
	// 5. draw

	wchar_t* chinese_str = L"董宾fgc";

	// clear screen
	clear_screen(BGRGB);

	// 从屏幕中心，画一条线
	for (int i = 0; i < 100; ++i)
		lcd_put_pixel(info.xres / 2 + i, info.yres / 2, 0xeebbff);
	
	// freetype 显示一个矢量字符
	// draw_a_Chinese_ft();
	// draw_Chinese_line(chinese_str);
	draw_Chinese_line_with_pen(chinese_str);

	// 5. close
	close_library();

	munmap(fb_base, screen_st);
	close(fb_fd);

	return 0;
}
