/**
 * @brief: draw a Chinese character on screen with freetype2 -- 22/7/2025
 *
 * @chapter:
 * 		+ 第四篇 嵌入式Linux应用开发基础知识
 * 			+ 【第六章】文字显示及图象显示
 * 				+ 6.5_使用freetype显示单个文字
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
 * 		目前自己写的例子，依然保留着从 Framebuffer 章节的每小节例子;
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
 *	
 * @output:
 * 		Framebuffer device: /dev/fb0 opened successfully!
 * 		Resolution(xres * yres): 1024 x 600, 32 bits per pixel.
 * 		one frame takes 2457600 bytes == 2400 KB == 2.34 MB.
 *
 * 		fb_base: 0x76a60000
 * 		hzk_base: 0x76a1e000
 * 		HZK16's size: 267616 bytes (261 KB), last access time: 2025/7/19 17:09, last modified time: 2025/7/19 17:09
 *
 * 		init_ft_library
 * 		Number of glyphs: 25185
 * 		This is a scalable font.
 * 		Units per EM: 1000
 * 		Number of fixed sizes: 0
 * 		Font format: TrueType
 * 		vector char doesn't need fixed size
 * 		x_ppem: 160
 * 		y_ppem: 160
 * 		
 * 		ASCII: K, start coordinates: (165, 266), rgb: 0x2d58c9
 * 		HZ: 0xa8b3, start coordinates: (544, 127), rgb: 0xc87373
 * 		HZ_UNI: 0x9526, start coordinates: (638, 50), rgb: 0x5170b8
 * 	
 * 		Glyph bitmap is successfully loaded
 * 		lcd_put_Chinese_ft x = 638, y = 50
 * 
 * @description:
 * 		这里 Freetype2 显示字符，也是在常见的汉字范围内(0x4e00 - 0x9fff)，随机生成一个 unicode 码;
 */
