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

/**
 * @brief 初始化编码操作结构体中的字体支持链表头
 *
 * 每个编码操作（Encoding Operator）可能支持多个字体操作对象。
 * 本函数初始化该编码操作结构体中的链表头，使其可以安全地添加字体操作对象。
 *
 * @param ptEncodingOpr 指向需要初始化字体支持链表的编码操作结构体指针
 *
 * @note 必须在向链表中添加字体操作对象之前调用此函数，
 *       否则调用 list_add 或 list_add_tail 时可能导致段错误（SIGSEGV）。
 */
inline void InitFontSupportedListHead(PT_EncodingOpr ptEncodingOpr) {
	INIT_LIST_HEAD(&ptEncodingOpr->tFontOprSupportedList);
}

int GetFileCoding(unsigned char* pucBufHead);
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
