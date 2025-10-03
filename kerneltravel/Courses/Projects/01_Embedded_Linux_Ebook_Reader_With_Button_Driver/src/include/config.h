#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <time.h>

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
#define DEBUG true
//#define DBG_PRINTF
#define DBG_PRINTF_MARK DBG_PRINTF("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__)

/* 函数声明 */
char* FormatTimeCustom(time_t timestamp, char* buf, int buf_len);

#endif 	// CONFIG_H
