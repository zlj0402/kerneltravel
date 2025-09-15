#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <time.h>
#include <string.h>

#define FB_DEV_NAME "/dev/fb0"

#define FREETYPE_FONT "freetype"
#define ASCII_FONT    "ascii"
#define GBK_FONT      "gbk"

#define NOT_TARGET_ENCODING 0
#define ASCII_ENCODING      1
#define UTF8_ENCODING       2
#define UTF16_LE_ENCODING   3
#define UTF16_BE_ENCODING   4

#define ASCII    "ascii"
#define UTF8     "utf8"
#define UTF16_LE "utf16-le"
#define UTF16_BE "utf16-be"

#define COLOR_BACKGROUND   0xE7DBB5  /* 泛黄的纸 */
#define COLOR_FOREGROUND   0x514438  /* 褐色字体 */

#define DBG_PRINTF printf
//#define DBG_PRINTF
#define DBG_PRINTF_MARK DBG_PRINTF("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__)

const char aStrUtf8[]	 = {0xEF, 0xBB, 0xBF, 0};
const char aStrUtf16le[] = {0xFF, 0xFE, 0};
const char aStrUtf16be[] = {0xFE, 0xFF, 0};

char* FormatTimeCustom(time_t timestamp, char* buf, int buf_len) {

    struct tm *tm = localtime(&timestamp); // 转换为本地时间结构体
    if (!tm) return NULL;

    // 格式化为 "2025/7/16 10:03"
    strftime(buf, buf_len, "%Y/%-m/%-d %-H:%M", tm);
    return buf;
}

/**
 * @brief 判断文件缓冲区的编码方式（ASCII/UTF-8/UTF-16 LE/UTF-16 BE）
 *
 * 通过比较文件头与 BOM (Byte Order Mark)，判断当前文本的编码方式。
 *
 * @param pucBufHead [in] 指向文件缓冲区开头的指针
 *
 * @return
 * - UTF8_ENCODING      如果是 UTF-8 编码
 * - UTF16_LE_ENCODING  如果是 UTF-16 Little Endian 编码
 * - UTF16_BE_ENCODING  如果是 UTF-16 Big Endian 编码
 * - ASCII_ENCODING     如果未检测到 BOM，默认认为是 ASCII
 */
int GetFileCoding(unsigned char* pucBufHead) {

	if (strncmp((const char*)pucBufHead, aStrUtf8, 3) == 0) {

		/* UTF-8 */
		return UTF8_ENCODING;
	}
	else if (strncmp((const char*)pucBufHead, aStrUtf16le, 2) == 0) {

		/* UTF16-LE */
		return UTF16_LE_ENCODING;
	}
	else if (strncmp((const char*)pucBufHead, aStrUtf16be, 2) == 0) {

		/* UTF16-BE */
		return UTF16_BE_ENCODING;
	}
	else {

		/* ASCII */
		return ASCII_ENCODING;
	}
}

#endif 	//CONFIG_H
