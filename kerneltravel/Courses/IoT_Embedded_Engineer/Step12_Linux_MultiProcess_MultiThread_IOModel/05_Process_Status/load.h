/**
 * @brief: 体验进程在接收到 kill（Ctrl + C） 或者 IO 信号（数据输入），进程状态的转变;
 * @author: Bin Dong
 * @date: 11/10/2025
 *
 * @output:
 * 	+ zlj@zlj-vm:~/zljgit/kerneltravel/Courses/IoT_Embedded_Engineer/Step12_Linux_MultiProcess_MultiThread_IOModel/05_Process_Status$ ./main
 * 	+ 56
 * 	+ zlj@zlj-vm:~/zljgit/kerneltravel/Courses/IoT_Embedded_Engineer/Step12_Linux_MultiProcess_MultiThread_IOModel/05_Process_Status$ ./main
 * 	+ ^C
 * 	+ zlj@zlj-vm:~/zljgit/kerneltravel/Courses/IoT_Embedded_Engineer/Step12_Linux_MultiProcess_MultiThread_IOModel/05_Process_Status$
 *
 * 	对于 Ctrl + C 和 接收到输入，虽是不同信号输入，但都由睡眠到就绪态;
 * 	进程是可中断的睡眠;
 */
