#ifndef DRAW_H
#define DRAW_H

#include <list.h>

typedef struct PageDesc {

	int iPage;
	unsigned char *pucLcdFirstPosAtFile;
	unsigned char *pucLcdNextPageFirstPosAtFile;
	struct list_head tList;
} T_PageDesc, *PT_PageDesc;

int SelectAndInitDisplay(char *pcName);
int OpenTextFile(char *pcTextFile);
int SetTextDetail(char *pcHZKFile, char *pcFileFreetype, unsigned int dwFontSize);
int ShowOnePage(unsigned char *pucTextFileMemCurPos);
int ShowNextPage(void);
int ShowPrePage(void);

#endif 	// DRAW_H