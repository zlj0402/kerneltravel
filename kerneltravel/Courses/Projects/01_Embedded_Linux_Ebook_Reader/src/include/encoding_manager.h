#ifndef ENCODING_MANAGER_H
#define ENCODING_MANAGER_H

#include <list.h>
#include <fonts_manager.h>

typedef struct EncodingOpr {

	char *name;
	int iHeadLen;
	struct list_head tFontOprSupportedList;
	int (*IsSupport)(unsigned char *pucBufHead);
	int (*GetCodeFrmBuf)(unsigned char *pucBufStart, unsigned char *pucBufEnd, unsigned int *pdwCode);
	struct list_head tList;	
} T_EncodingOpr, *PT_EncodingOpr;

int RegisterEncodingOpr(PT_EncodingOpr ptEncodingOpr);
void ShowEncodingOpr(void);
PT_EncodingOpr SelectEncodingOprForFile(unsigned char *pucFileBufHead);
int AddFontOprForEncoding(PT_EncodingOpr ptEncodngOpr, PT_FontOpr ptFontOpr);
int DelFontOprForEncoding(PT_EncodingOpr ptEncodingOpr, PT_FontOpr ptFontOpr);
int EncodingInit(void);

int AsciiEncodingInit();
int Utf16leEncodingInit();
int Utf16beEncodingInit();
int Utf8EncodingInit();


#endif 	//ENCODING_MANAGER_H
