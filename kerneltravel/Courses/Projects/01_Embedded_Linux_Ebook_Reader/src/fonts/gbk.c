#include <fonts_manager.h>
#include <config.h>

// fstat
#include <sys/stat.h>
// open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// mmap
#include <sys/mman.h>
// close
#include <unistd.h>

#define TIMESTRLEN 64


static int 			  g_iHZKFd;
static unsigned char *g_pucHZKMem;
static unsigned char *g_pucHZKMemEnd;

static char acTimeStr[TIMESTRLEN];


static int GBKFontInit(char *pcFontFile, unsigned int dwFontSize);

static int GBKGetFontBitmap(unsigned int dwCode, PT_FontBitmap ptFontBitmap);


/* 分配、设置、注册      T_FontOpr */
static T_FontOpr g_tGBKFontOpr = {

	.name          = GBK_FONT,
	.FontInit      = GBKFontInit,
	.GetFontBitmap = GBKGetFontBitmap,
};

/**
 * @brief Initialize GBK font from HZK16 file.
 *
 * This function opens the specified HZK16 font file, retrieves its metadata,
 * and memory-maps it into the process address space for fast access when
 * rendering GBK characters.
 *
 * @param pcFontFile Path to the HZK16 font file (GBK encoding).
 * @param dwFontSize Font size in pixels (currently fixed at 16x16 for HZK16).
 *
 * @retval 0 Success.
 * @retval -1 Failure (file open error, fstat error, or mmap error).
 */
static int GBKFontInit(char *pcFontFile, unsigned int dwFontSize) {

	struct stat tHZKStat;

	if (dwFontSize != 16) {
		DBG_PRINTF("GBK bitmap can't support %d fontsize, we'll use default 16 pixels\n", dwFontSize);
	}

	// 打开 pcFontFile(HZK16) 文件，映射这个文件地址
	g_iHZKFd = open(pcFontFile, O_RDONLY);
	if (g_iHZKFd < 0) {

		DBG_PRINTF("can not open %s\n", pcFontFile);
		return -1;
	}

	if (fstat(g_iHZKFd, &tHZKStat) == -1) {
		
		DBG_PRINTF("can not get %s info\n", pcFontFile);
		close(g_iHZKFd);
		return -1;
	}
	g_pucHZKMem = mmap(NULL, tHZKStat.st_size, PROT_READ, MAP_SHARED, g_iHZKFd, 0);
	if ((void*)g_pucHZKMem == MAP_FAILED) {
		
		DBG_PRINTF("%s mmap error happened.\n", pcFontFile);
		close(g_iHZKFd);
		return -1;
	}
	g_pucHZKMemEnd = g_pucHZKMem + tHZKStat.st_size;

	DBG_PRINTF("g_pucHZKMem: %p, g_pucHZKMemEnd: %p\n", (void*)g_pucHZKMem, (void*)g_pucHZKMemEnd);
	DBG_PRINTF("%s's size: %d bytes (%.3g KB), last access time: %s, last modified time: %s\n", 
		pcFontFile, (int)tHZKStat.st_size, tHZKStat.st_size / 1024.0, 
		FormatTimeCustom(tHZKStat.st_atime, acTimeStr, TIMESTRLEN),
		FormatTimeCustom(tHZKStat.st_mtime, acTimeStr, TIMESTRLEN));

	return 0;
}

/**
 * @brief Retrieve the bitmap data for a GBK character from the mapped font file.
 *
 * This function calculates the offset in the HZK16 font file based on the GBK
 * character code and returns the corresponding bitmap in @p ptFontBitmap.
 *
 * @param dwCode GBK-encoded character code (two bytes, e.g., 0xD6D0 for '中').
 * @param ptFontBitmap Pointer to the FontBitmap structure where bitmap info will be stored.
 *
 * @retval 0 Success — bitmap data is filled in @p ptFontBitmap.
 * @retval -1 Failure — invalid GBK code, unsupported code length, or out-of-range offset.
 *
 * @note The function currently only supports GBK two-byte codes (0xA1–0xFE range).
 */
static int GBKGetFontBitmap(unsigned int dwCode, PT_FontBitmap ptFontBitmap) {

	int iArea;
	int iWhere;

	int iPenX = ptFontBitmap->iCurOriginX;
	int iPenY = ptFontBitmap->iCurOriginY;

	DBG_PRINTF_MARK;

	if (dwCode & 0xffff0000) {
		
		DBG_PRINTF("don't support dwCode whose binary length is more than 8 bits: 0x%x\n", dwCode);
		return -1;
	}	

	iArea  = (int)(dwCode & 0xff) - 0xA1;
	iWhere = (int)((dwCode >> 8) & 0xff) - 0xA1;

	if ((iArea < 0) || (iWhere < 0)) {
		
		DBG_PRINTF("(iArea, iWhere) => (%d, %d) is not inside the range (> 0xa1, > 0xa1)\n", iArea, iWhere);
		return -1;
	}
	
	ptFontBitmap->iXLeft    = iPenX;
	ptFontBitmap->iYTop     = iPenY - 16;
	ptFontBitmap->iXMax     = iPenX + 16;
	ptFontBitmap->iYMax     = iPenY;
	ptFontBitmap->iBpp      = 1;
	ptFontBitmap->iPitch    = 2;
	ptFontBitmap->pucBuffer = g_pucHZKMem + (iArea * 94 + iWhere)*32;

	if (ptFontBitmap->pucBuffer >= g_pucHZKMemEnd) {
		
		DBG_PRINTF("converted dwCode %p is more than g_pucHZKMem range: [%p ~ %p]\n", 
									(void*)ptFontBitmap->pucBuffer, (void*)g_pucHZKMem, (void*)g_pucHZKMemEnd);
		return -1;
	}

	ptFontBitmap->iNextOriginX = iPenX + 16;
	ptFontBitmap->iNextOriginY = iPenY;
	
	return 0;
}

/**
 * @brief Register GBK font operations.
 *
 * This function registers the GBK font operation set with the global font
 * operation manager, enabling GBK font rendering in the system.
 *
 * @retval 0 Success.
 * @retval -1 Failure.
 */
int GBKInit(void) {
	return RegisterFontOpr(&g_tGBKFontOpr);
}

