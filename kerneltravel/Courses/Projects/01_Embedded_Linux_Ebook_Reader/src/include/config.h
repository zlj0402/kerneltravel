#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <time.h>

#define FB_DEV_NAME "/dev/fb0"

//#define DBG_PRINTF printf
#define DBG_PRINTF (...)

char* format_time_custom(time_t timestamp, char* buf, int buf_len) {

    struct tm *tm = localtime(&timestamp); // 转换为本地时间结构体
    if (!tm) return NULL;

    // 格式化为 "2025/7/16 10:03"
    strftime(buf, buf_len, "%Y/%-m/%-d %-H:%M", tm);
    return buf;
}


#endif 	//CONFIG_H
