/**
 * @brief: draw a Chinese character on screen with HZK16 -- 16/7/2025
 *
 * @description: 在能够画一条线的基础上 -- 上一节
 * 			+ mmap 映射 /dev/fb0 设备节点对应的 Framebuffer，得到映射内存的起始虚拟地址;
 * 				fb_base = mmap(NULL, screen_st, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
 * 				+ 第一个参数: 我们想要的映射的内存空间的起始虚拟地址;
 * 				+ 第二个参数: 屏幕显示数据的大小: 像素点数(xres * yres) * 每个像素值(2 3 个byte的RGB值)
 * 				+ 第三个参数: 映射页的权限申请，之后我们要读写的话就是: PROT_READ | PROT_WRITE
 * 				+ 第四个参数: 对内存的修改时，是原内存直接修改(MAP_SHARED)，还是在copy的那份上的修改(MAP_PRIVATE);
 * 				+ 第五个参数: file descriptor
 * 				+ 第六个参数: 相对于映射的内存地址的偏移，需要是 PAGE_SHIFT 的倍数;
 * 			+ 根据坐标，和像素占据的位数，赋值一个 RGB 值; 连续赋值 100 个;
 * 			---------------------------------------------------------------
 * 			+ 从内核的 /lib/font/font_8x16.c 中复制出 fontdata_8x16[]，做出一个头文件;
 * 				+ #include "", 也不用指定路径，把 fontdata_8x16.c 文件也编成 .o 文件
 * 				+ 然后，与 lcd_put_ascii.o 一起链接成 ./main 可执行文件;
 * 			+ 每个 ascii 码也是有 16 行数据表示，每行 8 个像素;
 * 			+ 像赋值每个像素点时一样，一个一个赋值就好了;
 * 			---------------------------------------------
 * 			+ 添加 lcd_put_Chinese --- 第四篇 - 第六章 - 6.3_中文字符的点阵显示
 * 									   嵌入式Linux电子书阅读器 - 03.2_字符的点阵显示_汉字字符
 * 				+ 下载 HZK16 的 GB2312 的字体点阵文件;
 * 				+ 在程序中打开该文件;
 * 					hzk_fd = open(HZK16, O_RDONLY);
 * 				+ 通过 stat/fstat 获取文件大小等信息;
 * 					fstat(hzk_fd, &statbuf)
 * 				+ mmap 映射一个内核空间，按照中文字符的区码和位码，找到字符索引;
 * 					hzk_base = mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED, hzk_fd, 0);
 * 					...
 * 					int area = (hz >> 8) - 0xa1;
 * 					int where = (hz & 0xff) - 0xa1;
 * 					unsigned char* char_base = hzk_base + (area * 94 + where) * 32;	// * 32 -> per Chinese char takes 32 bytes
 * 				+ 中文字符点阵两个字节一行，共 16 行; 给每个像素填充颜色即可;
 *
 * @steps:
 * 		下载 HZK16 文件，映射得到一个虚拟地址，根据中文字符的规则，找到索引，取出点阵信息，填充 framebuffer;
 * 		规则见:  第四篇 - 第六章 - 6.3_中文字符的点阵显示 
 *
 * @phen.:
 * 		在屏幕中心往右，划了一条线，100 个像素长度;
 * 		-------------------------------------------
 * 		随机在 lcd 的某个位置，画一个大写或小写的英文字母，并且颜色也是随机的;
 * 		----------------------------------------------------------------------
 *		随机在 lcd 的某个位置，画一个 0xa1 - 0xf7 区，0xa1 - 0xfe 位之间的字符，颜色也是随机的;
 *			ps: 区码 （0xa9 - 0xb0）之间，不是汉字字符区间
 */
