#ifndef DISP_MANAGER_H
#define DISP_MANAGER_H

#include <list.h>

typedef struct DispOpr {

	char *name;
	unsigned int uiXres;
	unsigned int uiYres;
	unsigned int uiBpp;
	struct list_head tList;
	int (*DeviceInit)(void); 
	int (*ShowPixel)(int iPenX, int iPenY, unsigned int);
	int (*CleanScreen)(unsigned int dwBackColor);
} T_DispOpr, *PT_DispOpr;

int RegisterDispOpr(PT_DispOpr ptDispOpr);
void ShowDispOpr(void);
PT_DispOpr GetDispOpr(char* pcName);
int DisplayInit(void);

int FBInit(void);

#endif 	//DISP_MANAGER_H
