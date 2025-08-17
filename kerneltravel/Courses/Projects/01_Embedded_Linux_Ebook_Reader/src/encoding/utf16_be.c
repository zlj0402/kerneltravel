#include <encoding_manager.h>
#include <config.h>

#include <string.h>

static int IsUtf16beCoding(unsigned char *pucBufHead);
static int Utf16beGetCodeFrmBuf(unsigned char *pucBufStart, unsigned char *pucBufEnd, unsigned int *pdwCode);

static T_EncodingOpr g_tUtf16beEncodingOpr = {
	
	.name          = UTF16_BE,
	.iHeadLen	   = 2,
	.IsSupport     = IsUtf16beCoding,
	.GetCodeFrmBuf = Utf16beGetCodeFrmBuf,
};

/**
 * @brief 判断文件是否为 UTF-16 BE 编码
 *
 * 该函数会调用 GetFileCoding，对文件缓冲区起始位置的数据进行判断，
 * 并确定其是否为 UTF-16 Big Endian (UTF-16 BE) 编码。
 *
 * @param pucBufHead 指向文件缓冲区起始位置的指针
 *
 * @return int
 * - 1 : 文件为 UTF-16 BE 编码
 * - 0 : 文件不是 UTF-16 BE 编码
 */
static int IsUtf16beCoding(unsigned char *pucBufHead) {
	return GetFileCoding(pucBufHead) == UTF16_BE_ENCODING ? 1 : 0;
}

/**
 * @brief 从缓冲区中解析一个 UTF-16 Big Endian 字符
 *
 * 该函数以 UTF-16 大端序方式读取两个字节，组合成一个 Unicode 码点。
 * 若缓冲区不足两个字节，则认为处理结束。
 *
 * @param pucBufStart [in] 缓冲区起始位置
 * @param pucBufEnd   [in] 缓冲区结束位置
 * @param pdwCode     [out] 返回解析出的 Unicode 码点
 *
 * @return
 * - 2 : 成功解析出一个 UTF-16BE 字符（两个字节）
 * - 0 : 缓冲区已处理完毕或数据不足
 */
static int Utf16beGetCodeFrmBuf(unsigned char *pucBufStart, unsigned char *pucBufEnd, unsigned int *pdwCode) {

	if (pucBufStart + 1 < pucBufEnd) {

		/* UTF16-BE */
		*pdwCode = ((unsigned int)pucBufStart[0] << 8) + pucBufStart[1]; 
		return 2;
	}

	/* 文件处理结束 */
	return 0;
}

/**
 * @brief 初始化 UTF-16BE 编码的字体操作结构
 *
 * 注册与 UTF-16BE 编码相关的字体操作（FreeType、ASCII），
 * 并将其加入到编码操作列表。
 *
 * @return
 * - 0   成功
 * - <0  注册失败
 */
// 既然只是编码，会什么不能挂在 GBK 字体下面?
int Utf16beEncodingInit(void) {

	AddFontOprForEncoding(&g_tUtf16beEncodingOpr, GetFontOpr(FREETYPE_FONT));
	AddFontOprForEncoding(&g_tUtf16beEncodingOpr, GetFontOpr(ASCII_FONT));

	return RegisterEncodingOpr(&g_tUtf16beEncodingOpr);
}

