/**
 * @brief: 一次按键异步信号读取一次键值; 实现了同时按下按键，但有一点问题;
 * @author: liangj.zhang
 * @date: 3/10/2025
 *
 * @version-match: src <===> button_old
 *
 * @features:
 * 		+ 使用异步信号的方式，接收到有按键事件后，信号处理函数 handler 去 /dev/reader_button 节点 read 一个值;
 * 			+ 做到上面这一点需要做到的:
 * 				+ 驱动:
 * 					+ file_operations.read 是无论何时都需要实现的;
 * 					+ file_operations.fasync:
 * 						static int gpio_key_drv_fasync(int fd, struct file *file, int on) {
 * 							if (fasync_helper(fd, file, on, &button_fasync) >= 0)
 * 								return 0;
 * 							else
 * 								return -EIO;
 * 						}
 * 						+ 总结:
 * 							gpio_key_drv_fasync() 用于在用户进程开启或关闭异步通知时,
 * 							调用 fasync_helper() 来维护异步通知进程链表。
 * 							之后驱动可通过 kill_fasync() 主动通知用户有新事件（例如按键按下）。
 * 				+ 应用:
 * 					+ 信号处理函数 handler: SigFunc, 根据 fd，读取一个值;
 * 					+ 在启动按键线程，开始异步接受信号之前，还有要做的步骤:
 * 						1. 注册一个信号处理函数: 
 * 							signal(SIGIO, SigFunc);
 * 							| 当进程收到 SIGIO 信号时，调用 SigFunc(int signo);
 * 						2. 设置“信号接收者”（即谁要收到 SIGIO）: 
 * 							fcntl(g_iDevFd, F_SETOWN, getpid());
 * 							| F_SETOWN：告诉内核，这个文件描述符 g_iDevFd 的信号由哪个进程接收;
 * 							| 意思是：“当设备有事件时，把 SIGIO 发给我这个进程。”
 * 						3. 读取设备文件当前的状态标志（比如 O_RDONLY, O_NONBLOCK 等）
 * 							int flags = fcntl(g_iDevFd, F_GETFL);
 * 						4. 在原有标志的基础上，启用 FASYNC 标志
 * 							fcntl(g_iDevFd, F_SETFL, flags | FASYNC);
 * 							| 这个动作非常关键，它会触发驱动的 .fasync 回调：
 * 							| gpio_key_drv_fasync(fd, file, 1);
 * 							| 驱动内部通过 fasync_helper() 把当前进程加入异步通知列表。
 * 							| 之后，当驱动检测到事件时会调用：
 * 							| kill_fasync(&button_fasync, SIGIO, POLL_IN);
 * 							| 于是内核给这个进程发 SIGIO，SigFunc() 被执行。
 * 					+ 子线程并没有做什么实质的内容，只是启动了 信号异步机制;
 * 		+ 除了基础的两个按键一个下一页，一个上一页;
 * 			还实现了同时按下两个按键，转换为 q 键值;
 * 		+ >>> 但这个版本并没有解决的问题，如下: <<<
 * 			1. 带 debug 输出的时候，页面的显示很慢的时候，此时按了 2 下以上，此页面显示完毕，接下来，只能处理一个按键值;
 * 			2. 因为目前按下一次按键，就会发送送号，应用程序从 /dev/reader_button 当中读取一个值，主线程的 while 轮询中会及时处理;
 * 				而同时按下两个按键值的逻辑，是一前一后两个按键值不同，且相差时间 < 50 ms，则得到一个 q 键值;
 * 				此键值得到，是在有 “一前” 的基础上，这个 “一前” 会被当做正常值就行按键处理，这是不好的;
 */
