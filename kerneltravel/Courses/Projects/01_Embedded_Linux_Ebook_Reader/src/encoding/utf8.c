#include <encoding_manager.h>
#include <config.h>

#include <string.h>
#include <limits.h>

static int IsUtf8Coding(unsigned char *pucBufHead);
static int Utf8GetCodeFrmBuf(unsigned char *pucBufStart, unsigned char *pucBufEnd, unsigned int *pdwCode);

static T_EncodingOpr g_tUtf8EncodingOpr = {
	
	.name          = UTF8,
	.iHeadLen	   = 3,
	.IsSupport     = IsUtf8Coding,
	.GetCodeFrmBuf = Utf8GetCodeFrmBuf,
};

/**
 * @brief 判断文件是否为 UTF-8 编码
 *
 * 该函数会调用 GetFileCoding，对文件缓冲区起始位置的数据进行判断，
 * 并确定其是否为 UTF-8 编码。
 *
 * @param pucBufHead 指向文件缓冲区起始位置的指针
 *
 * @return int
 * - 1 : 文件为 UTF-8 编码
 * - 0 : 文件不是 UTF-8 编码
 */
static int IsUtf8Coding(unsigned char *pucBufHead) {
	return GetFileCoding(pucBufHead) == UTF8_ENCODING ? 1 : 0;
}

/**
 * @brief 统计字节高位连续 '1' 的个数
 *
 * 用于解析 UTF-8 字节流，确定一个字符由多少个字节表示。
 *
 * 例如：
 * - 0xxxxxxx -> 返回 0，表示 ASCII 单字节
 * - 110xxxxx -> 返回 2，表示 2 字节序列的起始字节
 * - 1110xxxx -> 返回 3，表示 3 字节序列的起始字节
 * - 11110xxx -> 返回 4，表示 4 字节序列的起始字节
 *
 * @param ucVal [in] 输入字节
 *
 * @return 高位连续 '1' 的数量
 */
static int CountLeadingOnes(unsigned char ucVal) {

	int iCount = 0;
	// CHAR_BIT defined in <limits.h>
	for (int i = CHAR_BIT - 1; i >= 0; --i) {

		if (!(ucVal & ((unsigned char)1 << i)))
			break;
		++iCount;
	}

	return iCount;
}

/**
 * @brief 从缓冲区中解析一个 UTF-8 字符
 *
 * 根据 UTF-8 规则，解析 ASCII 或多字节 UTF-8 编码字符，
 * 并返回对应的 Unicode 码点。
 *
 * @param pucBufStart [in] 缓冲区起始位置
 * @param pucBufEnd   [in] 缓冲区结束位置
 * @param pdwCode     [out] 返回解析出的 Unicode 码点
 *
 * @return
 * - >0 : 成功解析出的字节数（1 表示 ASCII，2-4 表示 UTF-8 多字节字符）
 * - 0  : 缓冲区处理完毕或数据不足
 */
static int Utf8GetCodeFrmBuf(unsigned char *pucBufStart, unsigned char *pucBufEnd, unsigned int *pdwCode) {

#if 0
		对于UTF-8编码中的任意字节B，如果B的第一位为0，则B为ASCII码，并且B独立的表示一个字符;
		如果B的第一位为1，第二位为0，则B为一个非ASCII字符（该字符由多个字节表示）中的一个字节，并且不为字符的第一个字节编码;
		如果B的前两位为1，第三位为0，则B为一个非ASCII字符（该字符由多个字节表示）中的第一个字节，并且该字符由两个字节表示;
		如果B的前三位为1，第四位为0，则B为一个非ASCII字符（该字符由多个字节表示）中的第一个字节，并且该字符由三个字节表示;
		如果B的前四位为1，第五位为0，则B为一个非ASCII字符（该字符由多个字节表示）中的第一个字节，并且该字符由四个字节表示;
	
		因此，对UTF-8编码中的任意字节，根据第一位，可判断是否为ASCII字符;
		根据前二位，可判断该字节是否为一个字符编码的第一个字节; 
		UTF-8 规范里，允许的最大序列是 4 字节;
		根据前四位（如果前两位均为1），可确定该字节为字符编码的第一个字节，并且可判断对应的字符由几个字节表示;
		根据前五位（如果前四位为1），根据第 5 位，可判断编码是否有错误或数据传输过程中是否有错误。
#endif

	unsigned char ucVal;
	int iCount;
	unsigned int dwSum = 0;
	
	ucVal = pucBufStart[0];
	iCount = CountLeadingOnes(ucVal);

	if (pucBufStart >= pucBufEnd || pucBufStart + iCount >= pucBufEnd) {

		/* 文件结束 */
		return 0;
	}

	if (iCount == 0) {

		/* ASCII */
		*pdwCode = pucBufStart[0];
		return 1;
	}
	else {

		dwSum = (ucVal << iCount) >> iCount;
		for (int i = 1; i < iCount; ++i) {

			ucVal = pucBufEnd[i] & 0x3f;
			dwSum <<= 6;
			dwSum |= ucVal;
		}
	}

	return iCount;
}

/**
 * @brief 初始化 UTF-8 编码的字体操作结构
 *
 * 注册与 UTF-8 编码相关的字体操作（FreeType、ASCII），
 * 并将其加入到编码操作列表。
 *
 * @return
 * - 0   成功
 * - <0  注册失败
 */
// 既然只是编码，会什么不能挂在 GBK 字体下面?
int Utf8EncodingInit(void) {

	AddFontOprForEncoding(&g_tUtf8EncodingOpr, GetFontOpr(FREETYPE_FONT));
	AddFontOprForEncoding(&g_tUtf8EncodingOpr, GetFontOpr(ASCII_FONT));

	return RegisterEncodingOpr(&g_tUtf8EncodingOpr);
}

