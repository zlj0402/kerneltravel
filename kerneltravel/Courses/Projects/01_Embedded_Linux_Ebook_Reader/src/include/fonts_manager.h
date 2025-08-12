#ifndef FONTS_MANAGER_H
#define FONTS_MANAGER_H

#include <list.h>

typedef struct FontBitMap {
	int iXLeft;
	int iYTop;
	int iXMax;
	int iYMax;
	int iBpp;
	int iPitch;   /* 对于单色位图, 两行象素之间的跨度 */
	int iCurOriginX;
	int iCurOriginY;
	int iNextOriginX;
	int iNextOriginY;
	unsigned char *pucBuffer;
} T_FontBitmap, *PT_FontBitmap;

typedef struct FontOpr {

	char *name;
	int (*FontInit)(char *pcFontFile, unsigned int dwFontSize);
	int (*GetFontBitmap)(unsigned int dwCode, PT_FontBitmap ptFontBitmap);
	struct list_head tList;
} T_FontOpr, *PT_FontOpr;

int RegisterFontOpr(PT_FontOpr ptFontOpr);
void ShowFontOpr(void);
PT_FontOpr GetFontOpr(char *pcName);
int FontsInit(void);

int FreetypeInit(void);

#endif 	//FONTS_MANAGER_H
