// fstat open
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
// bool
#include <stdbool.h>
// strcmp
#include <string.h>
// open fstat
#include <fcntl.h>
// mmap
#include <sys/mman.h>

#include <config.h>
#include <list.h>
#include <encoding_manager.h>
#include <disp_manager.h>
#include <fonts_manager.h>
#include <draw.h>


static int g_iFdTextFile;
static unsigned char *g_pucTextFileMem;
static unsigned char *g_pucTextFileMemEnd;
static PT_EncodingOpr g_ptEncodingOprForFile;

/* 指向，当前页面的第一个字符位置 */
static unsigned char *g_pucLcdFirstPosAtFile;
/* 指向，下一个页面的第一个字符位置 */
static unsigned char *g_pucLcdNextPosAtFile;

static int g_dwFontSize;
static PT_DispOpr g_ptDispOpr;

static LIST_HEAD(g_tPagesHead);
static struct list_head *g_ptCurPage = &g_tPagesHead;

/**
 * @brief   打开一个文本文件，并建立内存映射，检测其编码格式
 * 
 * @param   pcTextFile  文本文件路径
 * @return  0   成功
 * @return -1   失败
 */
int OpenTextFile(char *pcTextFile) {

	struct stat tStat;

	g_iFdTextFile = open(pcTextFile, O_RDONLY);
	if (g_iFdTextFile < 0) {

		DBG_PRINTF("can't open text file %s\n", pcTextFile);
		return -1;
	}

	if (fstat(g_iFdTextFile, &tStat)) {

		DBG_PRINTF("can't get fstat\n");
		return -1;
	}

	g_pucTextFileMem = (unsigned char*)mmap(NULL, tStat.st_size, PROT_READ, MAP_SHARED, g_iFdTextFile, 0);
	if (g_pucTextFileMem == (unsigned char*)MAP_FAILED) {

		close(g_iFdTextFile);
		DBG_PRINTF("can't mmap text file %s\n", pcTextFile);
		return -1;
	}

	g_pucTextFileMemEnd = g_pucTextFileMem + tStat.st_size;

	g_ptEncodingOprForFile = SelectEncodingOprForFile(g_pucTextFileMem);

	if (g_ptEncodingOprForFile) {

		g_pucLcdFirstPosAtFile = g_pucTextFileMem + g_ptEncodingOprForFile->iHeadLen;
		return 0;
	}
	else {
		return -1;
	}
}

// 给字体对象，根据字体文件进行 init
int SetTextDetail(char *pcHZKFile, char *pcFileFreetype, unsigned int dwFontSize) {

	int error;
	struct list_head *pos;
	PT_FontOpr ptFontOpr;
	PT_FontOpr ptTmp;
	int iRet = -1;

	g_dwFontSize = dwFontSize;

	DBG_PRINTF("EncodingOpr(%s): FontOpr supported situation as following:\n", g_ptEncodingOprForFile->name);

	list_for_each(pos, &g_ptEncodingOprForFile->tFontOprSupportedList) {

		ptFontOpr = list_entry(pos, T_FontOpr, tList);
		
		if (strcmp(ptFontOpr->name, ASCII_FONT) == 0) {
			error = ptFontOpr->FontInit(NULL, dwFontSize);
		}
		else if (strcmp(ptFontOpr->name, GBK_FONT) == 0) {
			error = ptFontOpr->FontInit(pcHZKFile, dwFontSize);
		}
		else {
			error = ptFontOpr->FontInit(pcFileFreetype, dwFontSize);
		}

		DBG_PRINTF("\tFontOpr(%s) %s\n", ptFontOpr->name, error == 0 ? "supported" : "not supported");
	
		/**
		 * 比如，对于ascii 编码的文件，可能用 ascii 字体也可能用 gbk 字体，
		 * 所以，只要有一个 FontInit 成功，SetTextDetail 最终就返回成功;
		 * 且保留成功注册的 FontOpr，否则在该 EncodingOpr 的字体支持链表中删除该 FontOpr;
		 */
		if (error == 0) {
			iRet = 0;
		}
		else {
			DelFontOprForEncoding(g_ptEncodingOprForFile, ptFontOpr);
		}

		ptFontOpr = ptTmp;
	}

	return iRet;
}

int SelectAndInitDisplay(char *pcName) {

	int error;
	g_ptDispOpr = GetDispOpr(pcName);
	if (!g_ptDispOpr) {
		return -1;
	}

	error = g_ptDispOpr->DeviceInit();
	return error;
}

static inline int IncLcdY(int iY) {

	if (iY + g_dwFontSize < g_ptDispOpr->uiYres)
		return iY + g_dwFontSize;
	else
		return 0;
}

static int RelocateFontPos(PT_FontBitmap ptFontBitmap)
{
	int iLcdY;
	int iDeltaX;
	int iDeltaY;

	if (ptFontBitmap->iYMax > g_ptDispOpr->uiYres)
	{
		/* 满页了 */
		return -1;
	}

	/* 超过LCD最右边 */
	if (ptFontBitmap->iXMax > g_ptDispOpr->uiXres)
	{
		/* 换行 */		
		iLcdY = IncLcdY(ptFontBitmap->iCurOriginY);
		if (0 == iLcdY)
		{
			/* 满页了 */
			return -1;
		}
		else
		{
			/* 没满页 */
			iDeltaX = 0 - ptFontBitmap->iCurOriginX;
			iDeltaY = iLcdY - ptFontBitmap->iCurOriginY;

			ptFontBitmap->iCurOriginX  += iDeltaX;
			ptFontBitmap->iCurOriginY  += iDeltaY;

			ptFontBitmap->iNextOriginX += iDeltaX;
			ptFontBitmap->iNextOriginY += iDeltaY;

			ptFontBitmap->iXLeft += iDeltaX;
			ptFontBitmap->iXMax  += iDeltaX;

			ptFontBitmap->iYTop  += iDeltaY;
			ptFontBitmap->iYMax  += iDeltaY;;
			
			return 0;	
		}
	}
	
	return 0;
}

static int ShowOneFont(PT_FontBitmap ptFontBitmap)
{
	int x;
	int y;
	unsigned char ucByte = 0;
	int i = 0;
	int bit;
	
	if (ptFontBitmap->iBpp == 1)
	{
		for (y = ptFontBitmap->iYTop; y < ptFontBitmap->iYMax; y++)
		{
			i = (y - ptFontBitmap->iYTop) * ptFontBitmap->iPitch;
			for (x = ptFontBitmap->iXLeft, bit = 7; x < ptFontBitmap->iXMax; x++)
			{
				if (bit == 7)
				{
					ucByte = ptFontBitmap->pucBuffer[i++];
				}
				
				if (ucByte & (1<<bit))
				{
					g_ptDispOpr->ShowPixel(x, y, COLOR_FOREGROUND);
				}
				else
				{
					/* 使用背景色, 不用描画 */
					// g_ptDispOpr->ShowPixel(x, y, 0); /* 黑 */
				}
				bit--;
				if (bit == -1)
				{
					bit = 7;
				}
			}
		}
	}
	else if (ptFontBitmap->iBpp == 8)
	{
		for (y = ptFontBitmap->iYTop; y < ptFontBitmap->iYMax; y++)
			for (x = ptFontBitmap->iXLeft; x < ptFontBitmap->iXMax; x++)
			{
				//g_ptDispOpr->ShowPixel(x, y, ptFontBitMap->pucBuffer[i++]);
				if (ptFontBitmap->pucBuffer[i++])
					g_ptDispOpr->ShowPixel(x, y, COLOR_FOREGROUND);
			}
	}
	else
	{
		DBG_PRINTF("ShowOneFont error, can't support %d bpp\n", ptFontBitmap->iBpp);
		return -1;
	}
	return 0;
}

int ShowOnePage(unsigned char *pucTextFileMemCurPos) {

	int error;
	int iLen;
	unsigned char *pucBufStart;
	unsigned int dwCode;
	struct list_head *ptPos;
	PT_FontOpr ptFontOpr;
	T_FontBitmap tFontBitmap;

	//bool bHasGetCode = false;
	bool bHasNotClrScreen = true;

	tFontBitmap.iCurOriginX = 0;
	tFontBitmap.iCurOriginY = g_dwFontSize;
	pucBufStart = pucTextFileMemCurPos;

	while (true) {

		if (bHasNotClrScreen) {

			printf(">>> 清屏一次 <<<\n");
			/* ShowOnePage 时，先清一下屏 */
			g_ptDispOpr->CleanScreen(COLOR_BACKGROUND);
			bHasNotClrScreen = false;
		}

		iLen = g_ptEncodingOprForFile->GetCodeFrmBuf(pucBufStart, g_pucTextFileMemEnd, &dwCode);
		if (iLen == 0) {

			/* 文件结束：-1 -> 一次页面没处理就异常退出;    0 -> 正常退出; */
			//return bHasGetCode ? -1 : 0;

			return 0;
		}

		pucBufStart += iLen;
		DBG_PRINTF("%s %s %d, buf: %p, len = %d, dwCode = 0x%x\n", __FILE__, __func__, __LINE__, pucBufStart, iLen, dwCode);

		/**
		 * \n\r 在 windows 中两个一起表示回车换行
		 * \n\r -> 我们只处理 \n，----
		 */
		if (dwCode == '\n') {

			// g_pucLcdNextPosAtFile -> 这里可以不用赋值吗，后面显示一个字符后的赋值的 pucBufStart，并不会弄丢读了多少字符;
			g_pucLcdNextPosAtFile = pucBufStart;

			/* 回车换行 */
			tFontBitmap.iCurOriginX = 0;
			tFontBitmap.iCurOriginY = IncLcdY(tFontBitmap.iCurOriginY);
			if (tFontBitmap.iCurOriginY == 0) {
				/* 显示完当前一屏了 */
				return 0;
			}
			else 
				continue;
		}
		else if (dwCode == '\r')
			continue;
		else if (dwCode == '\t') {
			/* TAB 键用一个空格替代 */
			dwCode = ' ';
		}

		DBG_PRINTF("dwCode = 0x%x\n", dwCode);

		list_for_each(ptPos, &g_ptEncodingOprForFile->tFontOprSupportedList) {

			ptFontOpr = list_entry(ptPos, T_FontOpr, tList);
			error = ptFontOpr->GetFontBitmap(dwCode, &tFontBitmap);
			DBG_PRINTF("%s %s %d, ptFontOpr->name = %s, %d\n", __FILE__, __func__, __LINE__, ptFontOpr->name, error);

			if (error != 0)
				continue;

			if (RelocateFontPos(&tFontBitmap)) {

				/* 剩下的 LCD 空间不能满足显示这个字符 */
				return 0;
			}

			/* 显示一个字符 */
			if (ShowOneFont(&tFontBitmap)) {
				return -1;
			}

			/* 为下一个字符显示做准备 */
			tFontBitmap.iCurOriginX = tFontBitmap.iNextOriginX;
			tFontBitmap.iCurOriginY = tFontBitmap.iNextOriginY;
			g_pucLcdNextPosAtFile = pucBufStart;
			break;
		}
	}

	return 0;
}

int ShowNextPage(void) {

	int error;
	PT_PageDesc ptPage;
	unsigned char *pucTextFileMemCurPos;

	// 原 g_ptCurPage 只有在第一次时，是没有赋值的
	//pucTextFileMemCurPos = g_ptCurPage ?
	//			g_ptCurPage->pucLcdNextPageFirstPosAtFile : g_pucLcdFirstPosAtFile;
	// g_ptCurPage 不是 list head 头结点时 or 不是链表的最后一页，取其下一个
	pucTextFileMemCurPos = list_end(g_ptCurPage, &g_tPagesHead) ? g_pucLcdFirstPosAtFile :
							list_entry(g_ptCurPage, T_PageDesc, tList)->pucLcdNextPageFirstPosAtFile;
	
	error = ShowOnePage(pucTextFileMemCurPos);
	DBG_PRINTF("%s %s %d, %d\n", __FILE__, __func__, __LINE__, error);

	if (error == 0) {

		/* 拥有下一个页面的话，就返回，因为可能是从旧页面往后，重新翻页 */
		if (list_end(g_ptCurPage, &g_tPagesHead) && 
			list_is_head(g_ptCurPage, &g_tPagesHead)) {

			g_ptCurPage = g_ptCurPage->next;
			return 0;
		}

		/* 新的页面被第一次翻到，需要 malloc 一个新的 Page */
		ptPage = (PT_PageDesc)malloc(sizeof(T_PageDesc));
		if (ptPage) {

			ptPage->pucLcdFirstPosAtFile = pucTextFileMemCurPos;
			ptPage->pucLcdNextPageFirstPosAtFile = g_pucLcdNextPosAtFile;
			// 插入到页面的链表当中
			list_add_tail(&ptPage->tList, &g_tPagesHead);
			// 当前游标指向 malloc 出来的新页面，也即当前页面
			g_ptCurPage = g_ptCurPage->next;
			DBG_PRINTF("%s %d, pos = %p\n", __func__, __LINE__, &ptPage->pucLcdFirstPosAtFile);
			return 0;
		}
		else 
			return -1;
	}

	return error;
}

int ShowPrePage(void) {

	int error;

	DBG_PRINTF_MARK;

	// g_ptCurPage 本身不为 head，及 g_ptCurPage->prev 也不为 head，则

	if (list_is_head(g_ptCurPage, &g_tPagesHead) || 
			list_is_head(g_ptCurPage->prev, &g_tPagesHead)) {
		return -1;
	}

	PT_PageDesc ptPrePage = list_entry(g_ptCurPage->prev, T_PageDesc, tList);
	DBG_PRINTF("%s %d, pos = %p\n", __func__, __LINE__, ptPrePage->pucLcdFirstPosAtFile);
	error = ShowOnePage(ptPrePage->pucLcdFirstPosAtFile);

	if (error == 0) {

		DBG_PRINTF_MARK;
		g_ptCurPage = g_ptCurPage->prev;
	}

	return error;
}
