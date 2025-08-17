#include <config.h>

// printf / perror
#include <stdio.h>
// open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// ioctl
#include <sys/ioctl.h>
// struct fb_var_resolution
#include <linux/fb.h>
// memset
#include <string.h>


static int g_iFBFd;
static unsigned char* g_pucFbMem;

static struct fb_var_resolution g_tVar;
static unsigned int g_uiPixelWidth;
static unsigned int g_uiLineWidth;
static unsigned int g_uiScreenSize;

static int FBDeviceInit(void);
static int FBShowPixel(int iPenX, int iPenY, unsigned int dwColor);
static int FBCleanScreen(unsigned int dwBackColor);

/* 构造，设置，注册一个结构体 */
static T_DispOpr g_tFBDispOpr = {

	.name        = "fb", 
	.DeviceInit  = FBDeviceInit,
	.ShowPixel   = FBShowPixel,
	.CleanScreen = FBCleanScreen,
	/* 剩下的属性值，只有在初始化之后，才能够知道 & 并获取 */
};

/**
 * @brief  初始化帧缓冲设备，获取屏幕参数并映射显存地址
 * 
 * 该函数完成以下操作：
 * 1. 打开帧缓冲设备文件（如 /dev/fb0）；
 * 2. 通过 ioctl 获取屏幕分辨率和颜色深度等可变屏幕信息；
 *    根据屏幕参数计算每行字节数和整个屏幕所占内存大小；
 * 3. 使用 mmap 将帧缓冲映射到用户空间，以便直接读写；
 * 
 * @return int 
 *         成功返回 0，失败返回 -1。
 */
static int FBDeviceInit(void)
{
	// 1. 打开帧缓冲设备 -- open
	g_iFBFd = open(FB_DEV_NAME, O_RDWR);
	
	if (g_iFBFd < 0) {
		
		DBG_PRINTF("can not open %s\n", FB_DEV_NAME);
		return -1;
	}
	
	DBG_PRINTF("Framebuffer device: %s opened successfully!\n", FB_DEV_NAME);


	// 2. 得到屏幕信息 -- ioctl
	// Usually,  on  success  zero is returned.
	if (ioctl(g_iFBFd, FBIOGET_VSCREENINFO, &g_tVar)) {

		perror("Error reading screen resolution variable information.\n");
		close(g_iFBFd);
		return -1;
	}
	
	DBG_PRINTF("Resolution(xres * yres): %d x %d, %d bits per pixel.\n", g_tVar.xres, g_tVar.yres, g_tVar.bits_per_pixel);

	g_tFBDispOpr.uiXres = g_tVar.xres;
	g_tFBDispOpr.uiYres = g_tVar.yres;
	g_tFBDispOpr.uiBpp  = g_tVar.bits_per_pixel;

	g_uiPixelWidth = g_tVar.bits_per_pixel / 8;
	g_uiLineWidth = g_tVar.xres * g_uiPixelWidth;
	g_uiScreenSize = g_tVar.yres * g_uiLineWidth;

	DBG_PRINTF("one frame takes %d bytes == %d KB == %.3g MB.\n\n", g_uiScreenSize, g_uiScreenSize / 1024, g_uiScreenSize * 1.0 / 1024 / 1024);

	// 3. mmap 来获取 Framebuffer 在内核中映射地址（返回的是虚拟地址），我们来直接读写;
 	g_pucFbMem = mmap(NULL, screen_st, PROT_READ | PROT_WRITE, MAP_SHARED, g_iFBFd, 0);
	if ((void*)p_ucFbMem == MAP_FAILED) {	// MAP_FAILED -> (void*)-1
		
		perror("Framebuffer mmap error happened.\n");
		close(g_iFBFd);
		return -1;
	}

	DBG_PRINTF("g_pucFbMem: %p\n", (void*)g_pucFbMem);
}

/**
 * @brief   在帧缓冲上绘制一个像素点
 *
 * @param   iPenX     像素点的 X 坐标（以像素为单位）
 * @param   iPenY     像素点的 Y 坐标（以像素为单位）
 * @param   dwColor   像素点的颜色值（RGB 格式，通常为 0xRRGGBB 或 ARGB）
 *
 * @return  0 表示成功，非 0 表示失败（如坐标越界等）
 *
 * @note    本函数依赖帧缓冲初始化完成，应在调用 FBDeviceInit 后使用。
 */
static int FBShowPixel(int iPenX, int iPenY, unsigned int dwColor)
{
	if (iPenX >= g_tVar.xres || iPenY >= g_tVar.yres || iPenX < 0 || iPenY < 0) {

		DBG_PRINTF("(%d, %d) out of screen size\n", iPenX, iPenY);
		return -1;
	}

	unsigned char  *pucPen8 = g_pucFbMem + iPenY * g_uiLineWidth + iPenX * g_uiPixelWidth;
	unsigned short *pwPen_16 = (unsigned short*) pucPen8;
	unsigned int   *pdwPen_32 = (unsigned int*) pucPen8;

	unsigned short red, green, blue;

	switch (g_tVar.bits_per_pixel) {

		case 8:
		{	// 对于 8bpp，color 就不再表示 RBG 三原色了，这涉及调色板的概念，color 是调色板的值。
			*pucPen8 = dwColor;
			break;
		}
		case 16:
		{	// 把 red、green、blue 这三种 8 位颜色值，根据 RGB565 的格式，
			// 只保留 red 中的高 5 位、green 中的高 6 位、blue 中的高 5 位，组合成一个新的 16 位颜色值。
			red   = (dwColor >> 16) & 0xff;
			green = (dwColor >> 8) & 0xff;
			blue  = (dwColor) & 0xff;

			dwColor = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
			*pwPen_16 = dwColor;
			break;
		}
		case 32:
		{	// 对于 32bpp，颜色格式跟 color 参数一致，可以直接写入 Framebuffer。
			*pdwPen_32 = dwColor;
			break;
		}
		default:
		{
			printf("can not support %d bpp\n", g_tVar.bits_per_pixel);
			break;
		}
	}

	return 0;
}

/**
 * @brief  清空屏幕，用指定的背景颜色填充整个帧缓冲区域
 * 
 * 根据帧缓冲当前的颜色深度（bpp），采用不同的方式填充背景色：
 * - 8bpp：直接使用调色板索引值填充显存；
 * - 16bpp：将 24 位 RGB 值转换为 RGB565 格式后填充；
 * - 32bpp：直接以 32 位颜色值填充；
 * 
 * @param dwBackColor 背景颜色值。
 *        - 对于 8bpp，是调色板索引值；
 *        - 对于 16bpp，是 24 位 RGB 值（格式为 RGB565）；
 *        - 对于 32bpp，是 0xAARRGGBB 格式或直接的 32 位颜色值。
 * 
 * @return int 
 *         成功返回 0，失败（不支持的 bpp）返回 -1。
 */
static int FBCleanScreen(unsigned int dwBackColor)
{
	unsigned short red, green, blue;

	switch (g_tVar.bits_per_pixel) {

		case 8:
		{	// 对于 8bpp，color 就不再表示 RBG 三原色了，这涉及调色板的概念，color 是调色板的值。
			memset(g_pucFbMem, dwBackColor, g_uiScreenSize);
			break;
		}
		case 16:
		{	// 把 red、green、blue 这三种 8 位颜色值，根据 RGB565 的格式，
			// 只保留 red 中的高 5 位、green 中的高 6 位、blue 中的高 5 位，组合成一个新的 16 位颜色值。
			red   = (dwBackColor >> 16) & 0xff;
			green = (dwBackColor >> 8) & 0xff;
			blue  = (dwBackColor) & 0xff;

			dwBackColor = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);

			unsigned short *pwFbMemEnd  = (unsigned short*)(g_pucFbMem + g_uiScreenSize);
			unsigned short *pwFbPointer = (unsigned short*)(g_pucFbMem);
			while (pwFbPointer < pwFbMemEnd) {
				
				*pwFbPointer = dwBackColor;
				++pwFbPointer;
			}
			break;
		}
		case 32:
		{	// 对于 32bpp，颜色格式跟 color 参数一致，可以直接写入 Framebuffer。
			unsigned int *pdwFbMemEnd  = (unsigned int*)(g_pucFbMem + g_uiScreenSize);
			unsigned int *pdwFbPointer = (unsigned int*)(g_pucFbMem);
			while (pdwFbPointer < pdwFbMemEnd) {
				
				*pdwFbPointer = dwBackColor;
				++pdwFbPointer;
			}
			break;
		}
		default:
		{
			DBG_PRINTF("can not support %d bpp\n", g_tVar.bits_per_pixel);
			return -1;
		}
	}

	return 0;
}

int FBInit(void) 
{
	INIT_LIST_HEAD(&g_tFBDispOpr->tList);
	return registerDispOpr(&g_tFBDispOpr);
}


