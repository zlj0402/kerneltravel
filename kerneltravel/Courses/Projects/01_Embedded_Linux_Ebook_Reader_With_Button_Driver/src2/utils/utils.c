#include <time.h>

char* FormatTimeCustom(time_t timestamp, char* buf, int buf_len) {
	
    struct tm *tm = localtime(&timestamp); // 转换为本地时间结构体
    if (!tm) return NULL;

    // 格式化为 "2025/7/16 10:03"
    strftime(buf, buf_len, "%Y/%-m/%-d %-H:%M", tm);
    return buf;
}

