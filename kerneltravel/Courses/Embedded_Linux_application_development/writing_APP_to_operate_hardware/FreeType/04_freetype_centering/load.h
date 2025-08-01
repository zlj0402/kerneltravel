/**
 * @brief: display centering -- 1/8/2025
 *
 * @descripton:
 * 		显示的第一行，是屏幕正中;
 * 		第二行，是第一行的原点，往下一个 font_width;
 *
 *		FT_Get_Glyph 得到的 glyph 拷贝，是一样的，不一样的是:
 *		face->glyph 的 advance 的单位制是 26.6;
 *		拷贝得到的 glyph 的 advance 的单位制是 16.16;
 *		这个不同，给我这个入门新手，对理解这个对象的不同时，造成了什么曲折;
 */
