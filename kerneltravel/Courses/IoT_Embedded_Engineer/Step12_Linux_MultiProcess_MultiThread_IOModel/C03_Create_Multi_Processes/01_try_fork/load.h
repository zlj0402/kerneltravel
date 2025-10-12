/**
 * @brief: 创建一个子进程，并打印 HelloWorld
 * 			查看父子进程执行的效果，子进程从 fork() 之后，走的是与父进程相同的代码段;
 * @author: Bin Dong
 * @date: 13/10/2025
 *
 * @output:
 * 	+ 输出情况 1:
 * 	+ zlj@zlj-vm:~/zljgit/kerneltravel/Courses/IoT_Embedded_Engineer/Step12_Linux_MultiProcess_MultiThread_IOModel/C03_Create_Multi_Processes$ ./main
 * 	+ Hello Fork()
 * 	+ Hello Fork()
 * 	+ fork_res = 230132, pid = 230131
 * 	+ fork_res = 0, pid = 230132
 * 	+ 输出情况 2:
 * 	+ zlj@zlj-vm:~/zljgit/kerneltravel/Courses/IoT_Embedded_Engineer/Step12_Linux_MultiProcess_MultiThread_IOModel/C03_Create_Multi_Processes$ ./main
 * 	+ Hello Fork()
 * 	+ fork_res = 230134, pid = 230133
 * 	+ zlj@zlj-vm:~/zljgit/kerneltravel/Courses/IoT_Embedded_Engineer/Step12_Linux_MultiProcess_MultiThread_IOModel/C03_Create_Multi_Processes$ Hello Fork()
 * 	+ fork_res = 0, pid = 230134
 *
 *	+ 总结:
 *		+ 情况2: 父进程先结束，回到终端命令行的状态; 子进程接着输出;
 *		+ 情况1: 子进程先结束的情况;
 *
 * @summary:
 * 	1. 调用 fork() 创建子进程之后，父进程和子进程会并发执行
 * 	2. 子进程在fork() 之后开始执行
 * 	3. 父子进程的执行顺序由操作系统算法决定的，不是由程序本身决定
 * 	4. 子进程会拷贝父进程的地址空间的数据（copy-on-write)
 * 		4.1 子进程需要写入相同变量值时，会先拷贝一份在写入；
 * 		4.2 子进程会拷贝父进程地址空间的内容，包括缓冲区、文件描述符等
 */
