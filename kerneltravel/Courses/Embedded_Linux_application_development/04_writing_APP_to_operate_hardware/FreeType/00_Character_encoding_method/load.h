/**
 * @brief: Under the same program condition, the Chinese characters in the same string, are output different value;
 * 		+ Reason: the source file is saved by different charset;
 * 		+ Solution: compiled with specified charset, to a utf-8 executable file;
 *
 * @updated: porting on 10/7/2025
 *
 * @solution: gcc -finput-charset=GBK -fexec-charset=UTF-8 -o test_utf8_2 test_ansi.c
 * 				+ -finput-charset 以指定的字符集 GBK，解码源文件;
 * 				+ -fexec-charset 生成目标字符集 utf-8 的可执行文件
 *
 * @chapter: notion
 * 			+ 嵌入式linux
 * 				+ Projects
 * 					+ 嵌入式软件
 * 						+ 嵌入式Linux电子书阅读器
 * 							+ 02_字符的编码方式
 * 			output、Makefile 解释，详情见上述笔记;
 */
