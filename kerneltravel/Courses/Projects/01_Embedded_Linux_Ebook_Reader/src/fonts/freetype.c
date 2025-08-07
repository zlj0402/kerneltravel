
// fstat
#include <sys/stat.h>

// freetype
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_FONT_FORMATS_H 
#include FT_GLYPH_H

static FT_Library    g_tLibrary;
static FT_Face       g_tFace;
static FT_GlyphSlot  g_tSlot;

static FT_Vector pen;

static int            g_iTTFFd;
static unsigned char* g_ucTTFMem;
static struct stat    g_tTTFStat;


static int FreetypeFontInit(char *pcFontFile, unsigned int dwFontSize);

static int FreetypeGetFontBitmap(unsigned int dwCode, PT_FontBitmap ptFontBitmap);

/* 分配、设置、注册      T_FontOpr */
static T_FontOpr g_tFreetypeFontOpr = {

	.name          = "freetype",
	.FontInit      = FreetypeFontInit,
	.GetFontBitmap = FreetypeGetFontBitmap,
};

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
		printf("Strike %d: height=%d, width=%d, size=%ld, x_ppem=%d, y_ppem=%d\n",
			   i, size.height, size.width, size.size, size.x_ppem, size.y_ppem);
	}
	if (g_tFace->num_fixed_sizes == 0) {

		printf("vector char doesn't need fixed size\n");
	}

}

static int FreetypeFontInit(char *pcFontFile, unsigned int dwFontSize)
{
	FT_Error error;
	
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
	if (fstat(g_iTTFFd, &g_tTTFStat) == -1) {
		
		DBG_PRINTF("can not get %s info\n", pcFontFile);
		FT_Done_FreeType( g_tLibrary );
		return -1;
	}

	g_ucTTFMem = mmap(0, g_tTTFStat.st_size, PROT_READ, MAP_SHARED, g_iTTFFd, 0);
	if (g_ucTTFMem == (unsigned char*)MAP_FAILED) {

		close(g_iTTFFd);
		FT_Done_FreeType( g_tLibrary );
		return -1;
	}

	// 3. get face, using ttf file info
	error = FT_New_Memory_Face(g_tLibrary, g_ucTTFMem, g_tTTFStat.st_size, 0, &g_tFace);
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
	while (error = FT_Set_Pixel_Sizes(g_tFace, uiTmpFontSize, 0)) {
			
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
	ptFontBitmap->iNextOriginX = iPenX + g_tSlot->advance.x >> 6;
	ptFontBitmap->iNextOriginY = iPenY;
	ptFontBitmap->pucBuffer    = g_tSlot->bitmap.buffer;

	return 0;
}

int FreetypeInit(void) 
{
	return RegisterFontOpr(&g_tFreetypeFontOpr);
}

