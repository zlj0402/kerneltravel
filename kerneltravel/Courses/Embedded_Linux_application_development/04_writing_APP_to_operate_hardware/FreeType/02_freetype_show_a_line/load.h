/**
 * @brief: draw a Chinese line  on screen with freetype2 -- 23/7/2025
 *
 * @chapter:
 * 		+ 第四篇 嵌入式Linux应用开发基础知识
 * 			+ 【第六章】文字显示及图象显示
 * 				+ 6.6_使用freetype显示一行文字
 * 		+ 嵌入式Linux电子书阅读器
 * 			+ 05.2_在PC上测试freetype_显示汉字
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
 *	
 * @output:
 * 		[root@100ask:/mnt/04_CodingAppOperatesHW/FreeType/02_freetype_show_a_line]# ./main
 * 		Framebuffer device: /dev/fb0 opened successfully!
 * 		Resolution(xres * yres): 1024 x 600, 32 bits per pixel.
 * 		one frame takes 2457600 bytes == 2400 KB == 2.34 MB.
 * 		
 * 		fb_base: 0x769eb000
 * 	
 * 		init_ft_library
 * 		Number of glyphs: 25185
 * 		This is a scalable font.
 * 		Units per EM: 1000
 *		Number of fixed sizes: 0
 *		Font format: TrueType
 *		vector char doesn't need fixed size
 *		x_ppem: 64
 *		y_ppem: 64
 *		
 *		str len: 5
 *		UNI: 0x8463, pen: (19200, 25600), rgb: 0x850cf3
 *		lcd_put_Chinese_ft x = 239, y = 192
 *		UNI: 0x5bbe, pen: (15104, 25600), rgb: 0x850cf3
 *		lcd_put_Chinese_ft x = 174, y = 191
 *		UNI: 0x66, pen: (11008, 25600), rgb: 0x850cf3
 *		lcd_put_Chinese_ft x = 146, y = 199
 *		UNI: 0x67, pen: (9664, 25600), rgb: 0x850cf3
 *		lcd_put_Chinese_ft x = 119, y = 183
 *		UNI: 0x63, pen: (7808, 25600), rgb: 0x850cf3
 *		lcd_put_Chinese_ft x = 96, y = 198
 *			
 * @description:
 * 		用 freetype2 显示一行字符，在指定位置开始，输出指定字符（包含中文和英文）;
 * 		两种方式，一种是手动定位，一种是使用 freetype 的 pen;
 */
