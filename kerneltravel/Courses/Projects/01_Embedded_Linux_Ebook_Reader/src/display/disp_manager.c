
#include <config.h>
#include <disp_manager.h>

#include <string.h>

static LIST_HEAD_INIT(g_tDispListHead);

/**
 * @brief 注册一个显示操作结构体到全局显示操作链表中
 *
 * 将传入的显示操作结构体（Display Operator）添加到全局链表 g_tDispListHead 的尾部。
 * 如果传入的指针为 NULL，则注册失败。
 *
 * @param[in] ptDispOpr 指向待注册的显示操作结构体的指针
 *
 * @retval 0 表示注册成功
 * @retval -1 表示注册失败（参数为 NULL）
 *
 * @note
 * 该函数假设 g_tDispListHead 已经使用 LIST_HEAD 宏或 INIT_LIST_HEAD 宏完成初始化。
 */
int RegisterDispOpr(PT_DispOpr ptDispOpr) {

	if (!ptDispOpr)
		return -1;

	list_add_tail(&ptDispOpr->tList, &g_tDispListHead);
	
	return 0;
}

/**
 * @brief 显示所有已注册的显示操作结构体信息
 *
 * 遍历全局显示操作链表 g_tDispListHead，并依次打印每个已注册显示操作的名称及其索引编号。
 * 主要用于调试目的，以验证注册的显示操作列表是否正确。
 *
 * @note
 * 假设 g_tDispListHead 已使用 LIST_HEAD 或 INIT_LIST_HEAD 宏正确初始化，
 * 且其中所有节点均为有效的 T_DispOpr 类型。
 */
void ShowDispOpr(void) {

	int i = 0;
	struct list_head *pos;
	PT_DispOpr ptDispOpr;

	list_for_each(pos, &g_tDispListHead) {

		ptDispOpr = list_entry(pos, T_DispOpr, tList);
		printf("%02d %s\n", i++, ptDispOpr->name);
	}
}

/**
 * @brief 根据名称查找已注册的显示操作结构体
 *
 * 在全局显示操作链表 g_tDispListHead 中查找名称与 pcName 匹配的显示操作结构体。
 *
 * @param[in] pcName 要查找的显示操作名称字符串
 *
 * @return 指向匹配的显示操作结构体的指针（PT_DispOpr）
 * @retval NULL 如果未找到匹配的名称
 *
 * @note
 * 该函数使用 strcmp 进行字符串匹配，要求 pcName 与结构体中的 name 成员完全一致。
 * 假设 g_tDispListHead 已正确初始化，且链表中的各节点为有效的 T_DispOpr 实例。
 */
PT_DispOpr GetDispOpr(char* pcName) {

	struct list_head *pos;
	PT_DispOpr ptDispOpr;

	list_for_each(pos, &g_tDispListHead) {

		ptDispOpr = list_entry(pos, T_DispOpr, tList);
		if (strcmp(ptDispOpr->name, pcName) == 0)
			return ptDispOpr;
	}

	return NULL;
}

/**
 * @brief 初始化显示设备
 *
 * 调用底层帧缓冲（Framebuffer）初始化函数 `FBInit`，完成显示设备的基本初始化。
 *
 * @return 初始化结果
 * @retval 0 成功
 * @retval 非0 错误码（由 FBInit 返回）
 *
 * @note
 * 如果初始化失败，会通过 DBG_PRINTF 输出错误信息。
 * 该函数通常在程序启动或模块加载时调用，确保显示子系统准备就绪。
 */
int DisplayInit(void) {

	int error;
	
	error = FBInit();
	if (error)
		DBG_PRINTF("FBInit failed: %d\n", error);

	return error;
}
