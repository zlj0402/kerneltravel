#include <encoding_manager.h>
#include <config.h>

#include <string.h>

static int IsUtf16leCoding(unsigned char *pucBufHead);
static int Utf16leGetCodeFrmBuf(unsigned char *pucBufStart, unsigned char *pucBufEnd, unsigned int *pdwCode);

static T_EncodingOpr g_tUtf16leEncodingOpr = {
	
	.name          = UTF16_LE,
	.iHeadLen	   = 2,
	.IsSupport     = IsUtf16leCoding,
	.GetCodeFrmBuf = Utf16leGetCodeFrmBuf,
};

/**
 * @brief 判断文件是否为 UTF-16 LE 编码
 *
 * 该函数会调用 GetFileCoding，对文件缓冲区起始位置的数据进行判断，
 * 并确定其是否为 UTF-16 Little Endian (UTF-16 LE) 编码。
 *
 * @param pucBufHead 指向文件缓冲区起始位置的指针
 *
 * @return int
 * - 1 : 文件为 UTF-16 LE 编码
 * - 0 : 文件不是 UTF-16 LE 编码
 */
static int IsUtf16leCoding(unsigned char *pucBufHead) {
	return GetFileCoding(pucBufHead) == UTF16_LE_ENCODING;
}

/**
 * @brief 从缓冲区中解析一个 UTF-16 Little Endian 字符
 *
 * 该函数以 UTF-16 小端序方式读取两个字节，组合成一个 Unicode 码点。
 * 若缓冲区不足两个字节，则认为处理结束。
 *
 * @param pucBufStart [in] 缓冲区起始位置
 * @param pucBufEnd   [in] 缓冲区结束位置
 * @param pdwCode     [out] 返回解析出的 Unicode 码点
 *
 * @return
 * - 2 : 成功解析出一个 UTF-16LE 字符（两个字节）
 * - 0 : 缓冲区已处理完毕或数据不足
 */
// return number of bytes coped with
static int Utf16leGetCodeFrmBuf(unsigned char *pucBufStart, unsigned char *pucBufEnd, unsigned int *pdwCode) {

	if (pucBufStart + 1 < pucBufEnd) {

		/* UTF16-LE */
		*pdwCode = ((unsigned int)pucBufStart[1] << 8) + pucBufStart[0]; 
		return 2;
	}

	/* 文件处理结束 */
	return 0;
}

/**
 * @brief 初始化 UTF-16LE 编码的字体操作结构
 *
 * 注册与 UTF-16LE 编码相关的字体操作（FreeType、ASCII），
 * 并将其加入到编码操作列表。
 *
 * @return
 * - 0   成功
 * - <0  注册失败
 */
int Utf16leEncodingInit(void) {

	InitFontSupportedListHead(&g_tUtf16leEncodingOpr);
	AddFontOprForEncoding(&g_tUtf16leEncodingOpr, GetFontOpr(FREETYPE_FONT));
	AddFontOprForEncoding(&g_tUtf16leEncodingOpr, GetFontOpr(ASCII_FONT));

	return RegisterEncodingOpr(&g_tUtf16leEncodingOpr);
}


