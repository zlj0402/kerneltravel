#include <fonts_manager.h>
#include <config.h>

// fstat
#include <sys/stat.h>
// open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// mmap
#include <sys/mman.h>
// freetype
#include <ft2build.h>
// close
#include <unistd.h>

#include FT_FREETYPE_H
#include FT_FONT_FORMATS_H 
#include FT_GLYPH_H

static FT_Library    g_tLibrary;
static FT_Face       g_tFace;
static FT_GlyphSlot  g_tSlot;

static int            g_iTTFFd;
static unsigned char* g_pucTTFMem;


static int FreetypeFontInit(char *pcFontFile, unsigned int dwFontSize);

static int FreetypeGetFontBitmap(unsigned int dwCode, PT_FontBitmap ptFontBitmap);

/* 分配、设置、注册      T_FontOpr */
static T_FontOpr g_tFreetypeFontOpr = {

	.name          = "freetype",
	.FontInit      = FreetypeFontInit,
	.GetFontBitmap = FreetypeGetFontBitmap,
};

/**
 * @brief 打印当前字体的基本信息
 *
 * 包括字体是否可缩放、EM单位、字体格式以及所有固定尺寸的详细信息。
 * 如果是矢量字体而无固定尺寸，也会输出相关提示。
 *
 * @note 依赖全局字体对象 g_tFace 已经初始化。
 */
static void PrintFontInfo(void) {

	if (g_tFace->face_flags & FT_FACE_FLAG_SCALABLE) {
		
		printf("This is a scalable font.\n");
		printf("Units per EM: %d\n", g_tFace->units_per_EM);
	}

	printf("Number of fixed sizes: %d\n", g_tFace->num_fixed_sizes);
	const char* ccFontFormat = FT_Get_Font_Format(g_tFace);
	printf("Font format: %s\n", ccFontFormat);
	
	for (int i = 0; i < g_tFace->num_fixed_sizes; i++) {
		
		FT_Bitmap_Size size = g_tFace->available_sizes[i];
		printf("Strike %d: height=%d, width=%d, size=%d, x_ppem=%d, y_ppem=%d\n",
			   i, (int)size.height, (int)size.width, (int)size.size, (int)size.x_ppem, (int)size.y_ppem);
	}
	if (g_tFace->num_fixed_sizes == 0) {

		printf("vector char doesn't need fixed size\n");
	}

}

/**
 * @brief 初始化 FreeType 字体
 *
 * 从字体文件中加载字体数据，创建字体对象，并设置所需像素大小。
 * 同时打印字体基本信息，供调试使用。
 *
 * @param pcFontFile 字体文件路径（如 *.ttf）
 * @param dwFontSize 所需字体像素大小（高度）
 * @retval 0 初始化成功
 * @retval -1 初始化失败（如文件打开失败、内存映射失败、加载字体失败等）
 *
 * @note 成功后，全局变量 g_tFace、g_tSlot、g_tLibrary 等被赋值。
 */
static int FreetypeFontInit(char *pcFontFile, unsigned int dwFontSize)
{
	FT_Error error;
	struct stat tTTFStat;
	
	error = FT_Init_FreeType(&g_tLibrary);
	if (error) {

		DBG_PRINTF("Failed to initialize FreeType library: error code %d\n", error);
		return -1;
	}

	// 1. open ttf file
	g_iTTFFd = open(pcFontFile, O_RDONLY);
	if (g_iTTFFd < 0) {

		DBG_PRINTF("open %s error\n", pcFontFile);
		FT_Done_FreeType( g_tLibrary );
		return -1;
	}

	// 2. get ttf file info
	if (fstat(g_iTTFFd, &tTTFStat) == -1) {
		
		DBG_PRINTF("can not get %s info\n", pcFontFile);
		FT_Done_FreeType( g_tLibrary );
		return -1;
	}

	g_pucTTFMem = mmap(0, tTTFStat.st_size, PROT_READ, MAP_SHARED, g_iTTFFd, 0);
	if (g_pucTTFMem == (unsigned char*)MAP_FAILED) {

		close(g_iTTFFd);
		FT_Done_FreeType( g_tLibrary );
		return -1;
	}

	// 3. get face, using ttf file info
	error = FT_New_Memory_Face(g_tLibrary, g_pucTTFMem, tTTFStat.st_size, 0, &g_tFace);
	if (error) {
		
		fprintf(stderr, "Failed to load font from memory: %d\n", error);
		FT_Done_FreeType( g_tLibrary );
		return -1;
	}

	printf("Number of glyphs: %ld\n", g_tFace->num_glyphs);

	g_tSlot = g_tFace->glyph;

	// 3.2 print face info
	PrintFontInfo();

	// 4. set size
	unsigned int uiTmpFontSize = dwFontSize;
	while ((error = FT_Set_Pixel_Sizes(g_tFace, uiTmpFontSize, 0))) {
			
		DBG_PRINTF("Failed to set pixel sizes: %d\n", error);
		
		uiTmpFontSize = (uiTmpFontSize + 1) % g_tFace->units_per_EM;

		if (uiTmpFontSize == dwFontSize) {

			DBG_PRINTF("no suitable integer font size during 0 ~ units_per_EM(%u)\n", g_tFace->units_per_EM);
			return -1;
		}
	}
	printf("x_ppem: %d\n", g_tFace->size->metrics.x_ppem);
	printf("y_ppem: %d\n", g_tFace->size->metrics.y_ppem);

	printf("\n");
	return 0;

}

/**
 * @brief 获取指定字符的字体位图信息
 *
 * 使用 FreeType 加载并渲染指定 Unicode 字符，获取其位图和相关布局信息。
 *
 * @param dwCode Unicode 编码（例如汉字 '中' 是 0x4E2D）
 * @param ptFontBitmap 输出参数，填充对应字符的位图和位置等信息
 * @retval 0 获取成功
 * @retval -1 获取失败（例如字符未能加载或渲染失败）
 *
 * @note 位图是 1bpp 单色格式，适合嵌入式显示。
 */
static int FreetypeGetFontBitmap(unsigned int dwCode, PT_FontBitmap ptFontBitmap)
{
	int iPenX = ptFontBitmap->iCurOriginX;
	int iPenY = ptFontBitmap->iCurOriginY;
	
	// 加载 glyph 图像
    if (FT_Load_Char(g_tFace, dwCode, FT_LOAD_RENDER | FT_LOAD_MONOCHROME)) {
		
        DBG_PRINTF("Failed to FT_Load_Char character 0x%x!\n", dwCode);
        return -1;
    }

	// lcd coordinate
	// 左上角: (iXLeft, iYTop)
	// 右下角: (iXMax, iYMax) -> iYMax > iYTop
	ptFontBitmap->iXLeft       = iPenX + g_tSlot->bitmap_left;
	ptFontBitmap->iYTop        = iPenY - g_tSlot->bitmap_top;
	ptFontBitmap->iXMax        = ptFontBitmap->iXLeft + g_tSlot->bitmap.width;
	ptFontBitmap->iYMax        = ptFontBitmap->iYTop + g_tSlot->bitmap.rows;
	ptFontBitmap->iBpp         = 1;
	ptFontBitmap->iPitch       = g_tSlot->bitmap.pitch;
	ptFontBitmap->iNextOriginX = iPenX + (g_tSlot->advance.x >> 6);
	ptFontBitmap->iNextOriginY = iPenY;
	ptFontBitmap->pucBuffer    = g_tSlot->bitmap.buffer;

	return 0;
}

/**
 * @brief 初始化 FreeType 字体操作模块
 *
 * 将 FreeType 字体操作结构注册到字体操作框架中。
 *
 * @retval 0 注册成功
 * @retval -1 注册失败
 *
 * @note 通常在系统初始化阶段调用。
 */
int FreetypeInit(void) 
{
	return RegisterFontOpr(&g_tFreetypeFontOpr);
}

