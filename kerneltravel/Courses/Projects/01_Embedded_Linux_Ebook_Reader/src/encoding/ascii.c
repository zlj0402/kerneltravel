#include <encoding_manager.h>
#include <config.h>

#include <string.h>

static int IsAsciiCoding(unsigned char *pucBufHead);
static int AsciiGetCodeFrmBuf(unsigned char *pucBufStart, unsigned char *pucBufEnd, unsigned int *pdwCode);

static T_EncodingOpr g_tAsciiEncodingOpr = {

	.name          = "ascii",
	.iHeadLen      = 0,
	.IsSupport     = IsAsciiCoding,
	.GetCodeFrmBuf = AsciiGetCodeFrmBuf,
};

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
static int IsAsciiCoding(unsigned char *pucBufHead) {
	return GetFileCoding(pucBufHead);
}

/**
 * @brief 从缓冲区中解析一个字符编码（支持 ASCII / GBK）
 *
 * 该函数根据缓冲区中的字节判断当前字符是 ASCII 单字节编码，
 * 还是 GBK 双字节编码，并返回对应的码值。
 *
 * - 如果字节 < 0x80，则为 ASCII，返回 1 字节。
 * - 如果字节 >= 0x80 且缓冲区有足够的字节，则认为是 GBK，
 *   默认按 Little Endian 组合两个字节，返回 2 字节。
 * - 如果缓冲区数据可能损坏，依旧返回一个字节（防止解析中断）。
 * - 如果到达缓冲区末尾，返回 0 表示处理完成。
 *
 * @note 本实现未区分 Big Endian 与 Little Endian，仅假设为 Little Endian。
 *
 * @param pucBufStart [in] 缓冲区起始位置
 * @param pucBufEnd   [in] 缓冲区结束位置
 * @param pdwCode     [out] 返回解析出的字符编码值（ASCII 或 GBK）
 *
 * @return
 * - 1 : 成功解析出 1 个 ASCII 字符
 * - 2 : 成功解析出 1 个 GBK 字符
 * - 0 : 缓冲区处理完毕
 */
static int AsciiGetCodeFrmBuf(unsigned char *pucBufStart, unsigned char *pucBufEnd, unsigned int *pdwCode) {

	unsigned char *pucBuf = pucBufStart;
	unsigned char c = *pucBuf;

	if ((pucBuf < pucBufEnd) && (c < (unsigned char)0x80)) {

		/* ASCII - return length of code: 1 */
		*pdwCode = *pucBuf;
		return 1;
	} // 这里没分编码的方式是 low endian 还是 big endian
	else if ((pucBuf + 1) < pucBufEnd && (c >= (unsigned char)0x80)) {

		/* GBK - default little endian - return length of code: 2 */
		*pdwCode = pucBuf[0] + (((unsigned int)pucBuf[1]) << 8);
		return 2;
	}

	if (pucBuf < pucBufEnd) {

		/* 可能文件损坏，但依旧返回一个码，即使是错误的 */
		*pdwCode = (unsigned int)c;
		return 1;
	}

	/* 文件处理完毕 */
	return 0;
}

/**
 * @brief 初始化 ASCII 编码的字体操作结构
 *
 * 注册与 ASCII 编码相关的字体操作（FreeType、ASCII、GBK），
 * 并将其加入到编码操作列表。
 *
 * @return
 * - 0   成功
 * - <0  注册失败
 */
int AsciiEncodingInit(void) {

	AddFontOprForEncoding(&g_tAsciiEncodingOpr, GetFontOpr(FREETYPE_FONT));
	AddFontOprForEncoding(&g_tAsciiEncodingOpr, GetFontOpr(ASCII_FONT));
	AddFontOprForEncoding(&g_tAsciiEncodingOpr, GetFontOpr(GBK_FONT));
	
	INIT_LIST_HEAD(&g_tAsciiEncodingOpr.tList);
	return RegisterEncodingOpr(&g_tAsciiEncodingOpr);
}

