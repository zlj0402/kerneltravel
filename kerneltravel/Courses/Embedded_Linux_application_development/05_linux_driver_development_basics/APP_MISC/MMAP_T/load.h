/*
 * @brief: a simple mmap app test -- porting on 8/7/2025
 *
 * @chapter:
 * 		第五篇：嵌入式Linux驱动开发基础知识
 * 			【第二十章】驱动程序基石
 * 				09_mmap基础知识
 *
 * @description:
 * 	sleep lets app not end;
 * 	then, we can check: cat /proc/pid/maps
 *
 * @steps:
 * 	+ 执行: ./test 3&
 * 	+ 再执行: ./test 30&
 *
 * 	后台有两个进程:
 * 		zlj@zlj-vm:~/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/05_linux_driver_development_basics/APP_MISC/MMAP_T$ a's address = 0x4c3300, a's value = 3
 * 		zlj@zlj-vm:~/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/05_linux_driver_development_basics/APP_MISC/MMAP_T$ ./test 30&
 * 		zlj@zlj-vm:~/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/05_linux_driver_development_basics/APP_MISC/MMAP_T$ a's address = 0x4c3300, a's value = 30
 * 		zlj@zlj-vm:~/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/05_linux_driver_development_basics/APP_MISC/MMAP_T$ ls
 * 		load.h  Makefile  mmap_test.c  mmap_test.o  test
 * 		zlj@zlj-vm:~/zljgit/kerneltravel/Courses/Embedded_Linux_application_development/05_linux_driver_development_basics/APP_MISC/MMAP_T$ ps | grep test
 * 		   2579 pts/0    00:00:00 test
 * 		   2580 pts/0    00:00:00 test
 *	
 *	@description:
 *		+ -static -> 静态编译，把一些依赖也一同打包进来;
 *		+ 两个进程的 a 竟然拥有同一个地址; 
 *		+ 但 a 的值却不相同; => 值不同，说明 a 的地址肯定不同; 为什么 a 拥有相同的地址?
 *
 *		A: 两个虚拟地址，映射到不同的物理地址; 具体见 notion @chapter;
 */
