#ifndef ENCODING_MANAGER_H
#define ENCODING_MANAGER_H

#include <list.h>

typedef struct EncodingOpr {

	char *name;
	int iHeadLen;
	struct list_head tFontOprSupportedList;
	int (*IsSupport)(unsigned char *pucBufHead);
	int (*GetCodeFrmBuf)(unsigned char *pucBufStart, unsigned char *pucBufEnd, unsigned int *pdwCode);
	struct list_head tList;	
} T_EncodingOpr, *PT_EncodingOpr;

#endif 	//ENCODING_MANAGER_H
