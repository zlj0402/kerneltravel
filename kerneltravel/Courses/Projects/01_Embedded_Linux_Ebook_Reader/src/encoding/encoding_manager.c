#include <encoding_manager.h>

#include <string.h>

static LIST_HEAD(g_tEncodingOprHead);

int RegisterEncodingOpr(PT_EncodingOpr ptEncodingOpr) {

	if (!ptEncodingOpr)
		return -1;

	list_add_tail(&ptEncodingOpr->tList, &g_tEncodingOprHead);

	return 0;
}

void ShowEncodingOpr(void) {

	int i = 0;
	struct list_head *pos;
	PT_EncodingOpr ptEncodingOpr;

	list_for_each(pos, &g_tEncodingOprHead) {

		ptEncodingOpr = list_entry(pos, T_EncodingOpr, tList);
		printf("%02d %s\n", i++, ptEncodingOpr->name);
	}
}

PT_EncodingOpr SelectEncodingOprForFile(unsigned char *pucFileBufHead) {

	struct list_head* pos;
	PT_EncodingOpr ptEncodingOpr;

	list_for_each(pos, &g_tEncodingOprHead) {

		ptEncodingOpr = list_entry(pos, T_EncodingOpr, tList);
		if (ptEncodingOpr->isSupport(pucFileBufHead))
			return ptEncodingOpr;
	}

	return NULL;
}

int AddFontOprForEncoding(PT_EncodingOpr ptEncodngOpr, PT_FontOpr ptFontOpr) {

	if (!ptEncodngOpr || !ptFontOpr)
		return -1;

	PT_FontOpr ptFontOprCpy;

	ptFontOprCpy = malloc(sizeof(PT_FontOpr));
	if (!ptFontOprCpy) {

		DBG_PRINTF("%s %s %d malloc(sizeof(PT_FontOpr) error\n", __file__, __func__, __line__);
		return -1;
	}

	memcpy(ptFontOprCpy, ptFontOpr, sizeof(T_FontOpr));
	list_add_tail(&ptFontOprCpy->tList, &ptEncodngOpr->tFontOprSupportedList);

	return 0;
}

int DelFontOprForEncoding(PT_EncodingOpr ptEncodingOpr, PT_FontOpr ptFontOpr) {

	struct list_head *pos;
	PT_FontOpr ptFontOprTmp;

	list_for_each(pos, ptEncodingOpr->tFontOprSupportedList) {

		// 得到链表上的 FontOpr
		ptFontOprTmp = list_entry(pos, PT_FontOpr, tList);
		// 与参数上的相比较
		if (strcmp(ptFontOprTmp->name, ptFontOpr->name) == 0) {
			
			list_del(pos);
			free(pos);
			return 0;
		}
	}

	return -1;
}

int EncodingInit(void) {

	int error;

	error = xxxEncodingInit();
	if (error) {

		DBG_PRINTF("xxxEncodingInit failed: %d.\n", error);
		return error;
	}
}


