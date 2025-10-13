/**
 * @brief: 子进程会拷贝父进程的数据，和缓冲区;
 * @author: Bin Dong
 * @date: 13/10/2025
 *
 * @ps:
 * 	+ 两种情况对比:
 * 		+ fputs("HelloWorld", stdout);
 * 			+ 输出: HelloHelloWorldHelloWorld ==> write 输出一次 + fputs 输出两次
 * 		+ fputs("HelloWorld\n", stdout);
 * 			+ 输出: HelloHelloWorld           ==> write 输出一次 + fputs 输出一次
 *
 * @summary:
 * 	+ 标准输出（stdout）的缓冲区，在以下几种情况下会被刷新：
 * 		+ 遇到 \n 并且是行缓冲时（输出到终端）
 * 		+ 手动调用 fflush(stdout)
 * 		+ 缓冲区满了
 * 		+ 程序正常结束时，`exit()` 或 `return 0` 会自动调用 `fclose(stdout)`，这时会刷新缓冲区，而 `fork()` 本身不会刷新缓冲区。
 * 		所以父进程和子进程各自都有一份「未刷出的缓冲数据」，但还没实际写到终端。
 *
 * 	+ write 写入是不会经过 stdio 的缓冲区，也不会影响 fputs 的缓冲。
 * 	+ fputs 写的是标准 I/O 库的缓冲区（stdout）。
 */
