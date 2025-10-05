/**
 * @brief: 基于 <src2,button_old3>，在应用层实现了同时按下两个按键退出的功能，且不会实现第一个键的翻页，且流畅;
 * @author: liangj.zhang
 * @date: 5/10/2025
 *
 * @version-match: src3 <===> button_old4_src3
 *
 * @feature:
 * 		+ 如 @brief，
 * 		+ 已知问题，还剩下:
 * 			1. 同时按下两个键，有时候小于设定的20 jiffies，连续不同按键的时间小于设定的 20 jiffies，不能够转换，还是单独键处理，因为消费者快于生产者;
 * 				+ chat 推荐使用 pthread_cond_t (pthread 条件变量);
 */
