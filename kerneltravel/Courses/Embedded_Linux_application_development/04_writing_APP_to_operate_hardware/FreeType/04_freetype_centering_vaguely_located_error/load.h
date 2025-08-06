/**
 * @brief: strange error record (when draw two centering lines on screen with freetype2) -- 29/7/2025
 *
 * @descripton:
 * 		printf 输出的方式 debug，每次 FT_Load_Glyph 这里就出现 segmentation error 了，
 * 		之所以这么说，
 * 		是因为 FT_Load_Glyph 的返回值判断，if 分支和 else 分支都没有将 printf 语句内容输出出来：
 *
 * @chapter: 详情见 Notion
 * 		+ 嵌入式Linux电子书阅读器
 * 			+ Cases
 * 				+ segmentation fault 在真正错误位置前方停止printf输出
 */
