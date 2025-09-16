#include <config.h>

#include <time.h>
#include <string.h>

const char aStrUtf8[]   = {0xEF, 0xBB, 0xBF, 0};
const char aStrUtf16le[] = {0xFF, 0xFE, 0};
const char aStrUtf16be[] = {0xFE, 0xFF, 0};

char* FormatTimeCustom(time_t timestamp, char* buf, int buf_len) {
	
    struct tm *tm = localtime(&timestamp); // 转换为本地时间结构体
    if (!tm) return NULL;

    // 格式化为 "2025/7/16 10:03"
    strftime(buf, buf_len, "%Y/%-m/%-d %-H:%M", tm);
    return buf;
}

int GetFileCoding(unsigned char* pucBufHead) {

    if (strncmp((const char*)pucBufHead, aStrUtf8, 3) == 0) {
        return UTF8_ENCODING;
    } else if (strncmp((const char*)pucBufHead, aStrUtf16le, 2) == 0) {
        return UTF16_LE_ENCODING;
    } else if (strncmp((const char*)pucBufHead, aStrUtf16be, 2) == 0) {
        return UTF16_BE_ENCODING;
    } else {
        return ASCII_ENCODING;
    }
}
