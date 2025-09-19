#include <encoding_manager.h>
#include <config.h>
#include <list.h>

// printf
#include <stdio.h>
// strcpy memcpy
#include <string.h>
// malloc
#include <stdlib.h>


static LIST_HEAD(g_tEncodingOprHead);

/**
 * @brief 注册编码操作到全局编码操作链表
 *
 * 将一个编码操作（Encoding Operator）添加到全局链表中，用于后续编码检测和处理。
 *
 * @param ptEncodingOpr 指向要注册的编码操作结构体指针
 *
 * @retval 0   注册成功
 * @retval -1  参数无效
 */
int RegisterEncodingOpr(PT_EncodingOpr ptEncodingOpr) {

	if (!ptEncodingOpr)
		return -1;

	list_add_tail(&ptEncodingOpr->tList, &g_tEncodingOprHead);

	return 0;
}

/**
 * @brief 显示当前已注册的编码操作
 *
 * 遍历全局编码操作链表，并输出每个编码操作的编号和名称。
 */
void ShowEncodingOpr(void) {

	int i = 0;
	struct list_head *pos;
	PT_EncodingOpr ptEncodingOpr;

	list_for_each(pos, &g_tEncodingOprHead) {

		ptEncodingOpr = list_entry(pos, T_EncodingOpr, tList);
		printf("%02d %s\n", i++, ptEncodingOpr->name);
	}
}

/**
 * @brief 为文件选择合适的编码操作
 *
 * 根据文件缓冲区起始位置的内容，遍历已注册的编码操作，
 * 调用其 isSupport 方法判断是否支持该文件编码。
 *
 * @param pucFileBufHead 文件缓冲区起始位置指针
 *
 * @return PT_EncodingOpr
 * - 成功：返回匹配的编码操作指针
 * - 失败：返回 NULL
 */
PT_EncodingOpr SelectEncodingOprForFile(unsigned char *pucFileBufHead) {

	struct list_head* pos;
	PT_EncodingOpr ptEncodingOpr;

	list_for_each(pos, &g_tEncodingOprHead) {

		ptEncodingOpr = list_entry(pos, T_EncodingOpr, tList);
		if (ptEncodingOpr->IsSupport(pucFileBufHead)) {

			DBG_PRINTF("%s encoding is selected.\n", ptEncodingOpr->name);
			return ptEncodingOpr;
		}
	}

	return NULL;
}

/**
 * @brief 为指定编码操作添加字体操作
 *
 * 将一个字体操作复制并添加到指定编码操作的支持列表中。
 *
 * @param ptEncodingOpr 指向编码操作结构体的指针
 * @param ptFontOpr     指向字体操作结构体的指针
 *
 * @retval 0   添加成功
 * @retval -1  参数无效或内存分配失败
 */
int AddFontOprForEncoding(PT_EncodingOpr ptEncodngOpr, PT_FontOpr ptFontOpr) {

	if (!ptEncodngOpr || !ptFontOpr)
		return -1;

	PT_FontOpr ptFontOprCpy;

	ptFontOprCpy = malloc(sizeof(T_FontOpr));
	if (!ptFontOprCpy) {

		DBG_PRINTF("%s %s %d malloc(sizeof(PT_FontOpr) error\n", __FILE__, __func__, __LINE__);
		return -1;
	}

	memcpy(ptFontOprCpy, ptFontOpr, sizeof(T_FontOpr));
	list_add_tail(&ptFontOprCpy->tList, &ptEncodngOpr->tFontOprSupportedList);

	return 0;
}

/**
 * @brief 从指定编码操作中删除字体操作
 *
 * 在编码操作的字体操作支持列表中查找目标字体操作，
 * 若找到则从链表中删除并释放内存。
 *
 * @param ptEncodingOpr 指向编码操作结构体的指针
 * @param ptFontOpr     指向要删除的字体操作结构体的指针
 *
 * @retval 0   删除成功
 * @retval -1  未找到目标字体操作
 */
int DelFontOprForEncoding(PT_EncodingOpr ptEncodingOpr, PT_FontOpr ptFontOpr) {

	struct list_head *pos;
	PT_FontOpr ptFontOprTmp;

	list_for_each(pos, &ptEncodingOpr->tFontOprSupportedList) {

		// 得到链表上的 FontOpr
		ptFontOprTmp = list_entry(pos, T_FontOpr, tList);
		// 与参数上的相比较
		if (strcmp(ptFontOprTmp->name, ptFontOpr->name) == 0) {
			
			list_del(pos);
			free(pos);
			return 0;
		}
	}

	return -1;
}

/**
 * @brief 初始化编码模块
 *
 * 调用具体的编码初始化函数（如 xxxEncodingInit），
 * 并检查其返回值以确认是否初始化成功。
 *
 * @retval 0   初始化成功
 * @retval <0  初始化失败，返回对应错误码
 */
int EncodingInit(void) {

	int error;

	error = AsciiEncodingInit();
	if (error) {

		DBG_PRINTF("AsciiEncodingInit failed: %d.\n", error);
		return error;
	}

	error = Utf16leEncodingInit();
	if (error) {

		DBG_PRINTF("Utf16leEncodingInit failed: %d.\n", error);
		return error;
	}

	error = Utf16beEncodingInit();
	if (error) {

		DBG_PRINTF("Utf16beEncodingInit failed: %d.\n", error);
		return error;
	}

	error = Utf8EncodingInit();
	if (error) {

		DBG_PRINTF("Utf8EncodingInit failed: %d.\n", error);
		return error;
	}

	return 0;
}

