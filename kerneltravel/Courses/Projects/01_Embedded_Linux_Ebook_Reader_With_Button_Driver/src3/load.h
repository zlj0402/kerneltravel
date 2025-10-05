/**
 * @brief: 基于 <src,button_old>，实现了一次能处理多个按键值（在一页显示很慢的时候）
 * @author: liangj.zhang
 * @date: 4/10/2025
 *
 * @version-match: src2 <===> button_old2
 *
 * @ps:
 * 	+ 这一版本，直接省去了 异步信号接收处理按键值;
 * 		直接在子线程中，进行 poll & read 读取;
 *
 * @feature:
 * 		+ 如 @brief，
 * 		+ 已知问题，还剩下:
 * 			1. 同时按下两个键，第一个键会被按正常按键值处理;
 * */
