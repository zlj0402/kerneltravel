#ifndef BUTTON_THREAD_H
#define BUTTON_THREAD_H

#include <stdbool.h>

// 初始化按键线程，传入设备节点，返回线程创建是否成功
bool ButtonThreadStart(const char *dev_node);

// 停止按键线程
void ButtonThreadStop(void);

// 获取最新按键事件，如果有按键返回 1 并通过 val 输出按键，没按键返回 0
int ButtonGetKey(char *val);

#endif // BUTTON_THREAD_H
