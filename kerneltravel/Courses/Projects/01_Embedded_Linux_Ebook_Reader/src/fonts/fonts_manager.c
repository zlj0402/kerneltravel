#include <fonts_manager.h>

#include <stdio.h>
#include <string.h>

static LIST_HEAD(g_tFontOprHead);

/**
 * @brief 注册一个字体操作对象
 *
 * 将传入的字体操作结构体添加到字体操作链表中。
 *
 * @param ptFontOpr 指向字体操作结构体的指针
 * @retval 0 注册成功
 * @retval -1 注册失败（例如传入空指针）
 */
int RegisterFontOpr(PT_FontOpr ptFontOpr) {

	if (!ptFontOpr)
		return -1;

	list_add_tail(&ptFontOpr->tList, &g_tFontOprHead);

	return 0;
}

/**
 * @brief 显示当前已注册的字体操作对象
 *
 * 遍历字体操作链表，打印出每个字体操作的名称。
 */
void ShowFontOpr(void) {

	int i = 0;
	struct list_head *pos;
	Pt_FontOpr ptFontOpr;

	list_for_each(pos, &g_tFontOprHead) {

		ptFontOpr = list_entry(pos, T_FontOpr, tList);
		printf("%02d %s\n", i++, ptFontOpr->name);
	}
}

/**
 * @brief 获取指定名称的字体操作对象
 *
 * 在字体操作链表中查找名称匹配的字体操作结构体。
 *
 * @param pcName 需要查找的字体操作名称
 * @return PT_FontOpr 指向匹配的字体操作结构体的指针
 * @retval NULL 未找到匹配的字体操作
 */
PT_FontOpr GetFontOpr(char *pcName) {

	struct list_head *pos;
	Pt_FontOpr ptFontOpr;

	list_for_each(pos, &g_tFontOprHead) {

		ptFontOpr = list_entry(pos, T_FontOpr, tList);
		if (strcmp(ptFontOpr->name, pcName) == 0)
			return ptFontOpr;
	}

	return NULL;
}

/**
 * @brief 初始化字体系统
 *
 * 调用 FreeType 字体引擎的初始化函数。
 *
 * @retval 0 初始化成功
 * @retval -1 初始化失败
 */
int FontsInit(void) {

	int error;

	error = FreetypeInit();
	if (error) {
		
		DBG_PRINTF("FreetypeInit failed: %d!\n", error);
		return -1;
	}

	return 0;
}
