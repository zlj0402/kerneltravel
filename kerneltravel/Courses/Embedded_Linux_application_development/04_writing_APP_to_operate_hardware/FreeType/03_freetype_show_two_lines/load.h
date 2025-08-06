/**
 * @brief: draw two Chinese lines on screen with freetype2 -- 27/7/2025
 *
 * @chapter:
 * 		+ 第四篇 嵌入式Linux应用开发基础知识
 * 			+ 【第六章】文字显示及图象显示
 * 				+ 6.6_使用freetype显示一行文字
 * 		+ 嵌入式Linux电子书阅读器
 * 			+ 07.1_在LCD上显示多行文字_从左显示
 *
 * @reference:
 * 		用的 freetype 版本，是当前最新的版本: freetype-2.13.3
 * 		+ 下载链接: https://download.savannah.gnu.org/releases/freetype/
 * 			+ 源码和 docs 是单独的压缩包，单独下载;
 * 			+ docs 目前主要参考的是:
 * 				+ freetype-2.13.3\docs\tutorial 下面的 example\index.html\step1.html
 *
 * @steps:
 * 		这节例子，已经删除前面的章节，只保留此节涉及的内容;
 * 		此小节:
 * 		+ 1. 包含头文件
 * 			+ #include <ft2build.h>
 * 				ft2build.h 不是普通头文件，它内部定义了一些宏，目的是为了帮助你以统一的方式引入 FreeType 提供的各个公共头文件。
 * 				这样以后只用：#include <Micro>
 * 					#include FT_FREETYPE_H => #include <freetype2/freetype/freetype.h>
 * 		+ 2. 初始化 Freetype 库
 * 			+ FT_Library library;
 * 			+ FT_Init_FreeType(&library);
 * 		+ 3. 加载字体 Face
 * 			+ FT_Face face;
 * 			+ 两种常见方式:
 * 				+ 直接以指定路径的字体文件:
 * 					error = FT_New_Face(library, "/usr/share/fonts/truetype/arial.ttf", 0, &face);
 * 				+ 字体文件打开后，有个地址:
 * 					+ 我是通过 mmap 根据这个字体文件的 fd & size 在内核映射了一个地址(字体文件的 size，可以根据 stat/fstat 获取到)
 *						ttf_base = mmap(0, ttf_stat.st_size, PROT_READ, MAP_SHARED, ttf_fd, 0);
 *					+ 然后根据该虚拟地址，加载字体 Face;
 *						error = FT_New_Memory_Face(library, ttf_base, ttf_stat.st_size, 0, &face);
 *		+ 4. 设置字体大小
 *			+ 两种常见方式: 
 *				+ error = FT_Set_Char_Size(
 *					face,    // face 对象句柄
 *					0,       // 字符宽度，以 1/64 点为单位
 *					16*64,   // 字符高度，以 1/64 点为单位，16pt
 *					300,     // 水平设备分辨率，单位 dpi
 *					300 );   // 垂直设备分辨率，单位 dpi
 *					+ 宽度/高度，一方为 0, 表示与另一方相同
 *				+ error = FT_Set_Pixel_Sizes(
 *					face,   // face 对象句柄
 *					0,      // pixel_width
 *					16 );   // pixel_height
 *		不在旋转等效果，最简单的字符渲染，初始化的步骤，只有这 4 步就可以了;
 *		--------------------------------------------------------------------
 *		字符写入控制方式:
 *		1. 手动控制，字符的写入位置
 *			void draw_Chinese_line(wchar_t* str)
 *				lcd_put_Chinese_ft(&slot->bitmap, char_x, char_y, wchar, char_color);
 *				char_x += slot->advance.x >> 6;
 *				char_y += slot->advance.y >> 6;
 *		2. FT_Vector pen 指定位置
 *			void draw_Chinese_line_with_pen(wchar_t* str)
 *				double angle		  = ( 180.0 / 360 ) * 3.14159 * 2;	//  旋转 180 度
 *
 *				FT_Matrix matrix;
 *				matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
 *				matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
 *				matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
 *				matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );
 *
 *				// 起始绘制位置（注意：单位是 1/64 像素，FreeType 用 fixed point）
 *				FT_Vector pen;
 *				pen.x = LENGTH * 64;                     // X: 从屏幕左侧 LENGTH 像素位置开始
 *				pen.y = (info.yres - HEIGHT) * 64;       // Y: 屏幕高度 - HEIGHT，坐标系向下
 *
 *				for (size_t i = 0; i < len; ++i)
 *					unsigned int wchar = (unsigned int)str[i];
 *					// 设置变换（包括位置）
 *					FT_Set_Transform(face, &matrix, &pen);
 *					// 加载 glyph 图像
 *					if (FT_Load_Char(face, wchar, FT_LOAD_RENDER)) {...}
 *
 *					slot = face->glyph;
 *
 *					// 注意：slot->bitmap_top 是相对 pen.y 的偏移（FreeType 坐标）
 *					int draw_x = slot->bitmap_left;
 *					int draw_y = info.yres - slot->bitmap_top;  // 从顶部向下绘制（像素坐标）
 *
 *					lcd_put_Chinese_ft(&slot->bitmap, draw_x, draw_y, wchar, char_color);
 *
 *					// 更新 pen 位置, 得到下一个字符的原点
 *					pen.x += slot->advance.x;  // advance.x 单位是 1/64 像素
 *					pen.y += slot->advance.y;
 *					-------------------------
 *		+ 2025/7/27 --- 嵌入式Linux电子书阅读器 - 07.1_在LCD上显示多行文字_从左显示
 *			FT_Glyph glyph;
 *			FT_Get_Glyph(face->glyph, &glyph); // 获取字形对象
 *
 *			FT_BBox bbox;
 *			FT_Glyph_Get_CBox(glyph, ft_glyph_bbox_truncate, &bbox);
 *			...
 *			if (line_box_ymin > bbox.yMin)
 *				line_box_ymin = bbox.yMin;
 *			if (line_box_ymax < bbox.yMax)
 *				line_box_ymax = bbox.yMax;
 *
 *			FT_Get_Glyph 从 face->glyph 中获取字形对象
 *			FT_Glyph_Get_CBox 用来获取字形的边界框（bounding box），即该字形在 x 和 y 方向上的 最小和最大值
 *			if 分支不断更新当前行的垂直边界，知道当前行的字符 在垂直方向上所占的空间范围;
 *			这样，后面显示新的一行时，第一个字符的原点，可以拿上一行第一个字符的原点，减去（ymax - ymin）的宽度，得到当前行的纵向起始写入位置;
 *			这样，可以避免该行与上一行重叠;
 *
 *			+ main
 *				// 起始绘制位置（注意：单位是 1/64 像素，FreeType 用 fixed point）
 *				pen.x = 0 * 64;                     		// X: 从屏幕左侧 0 像素位置开始
 *				pen.y = (info.yres - font_width - 160) * 64;      // Y: 屏幕高度 - HEIGHT，坐标系向下
 *				draw_Chinese_line_with_pen(chinese_str);
 *
 *				pen.x = 0 * 64;
 *				pen.y -= (line_box_ymax - line_box_ymin) * 64;
 *				draw_Chinese_line_with_pen(sec_line);
 *
 *				ps: 如果第一行都是英文字符，第二行是汉字，汉字的字符高度一般都是较高的，这是会重叠;
 *					我的想法是，>>> 直接减去设定的字符 pixel size; <<<
 *			
 * @description:
 * 		用 freetype2 显示两行字符，在指定位置开始，输出指定字符（包含中文和英文）;
 * 		根据第一行字符的宽度，来决定第二行的纵向起始位置，同语言文本的话，可以避免字符重叠;
 */
