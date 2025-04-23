/*
 * @brief: button drv demo
 * 
 * @description:
 * 		+ 没有用 platform_device 和 platform_driver
 * 		+ 只是简单的注册了个 file_operations 驱动程序
 * 		---------------------------------------------
 * 		+ 我一开始，没有理解这次的 BUTTON 的特别之处，特意重新，认真的看了一下配套的 第5篇第14章 的教程 pdf
 * 			+ 发现，这节没有特别之处，要说特别，见下方:
 * 			+ BUTTON 意思是：从引脚读取; 
 * 				+ 这个 demo 也没有给出 poll/select 等驱动程序响应系统调用的函数;
 * 				+ 真有个 BUTTON 被按下，那也是去 read 引脚;
 * 				+ 该章节重点是 read;
 *
 * @steps:
 * 		+ 执行: make
 * 		+ 将 board_xxx.ko button_drv.ko button_test 一起，拷贝到 ubuntu 和开发板共享的目录;
 * 		+ 先 insmod button_drv.ko，后 insmod board_xxx.ko;
 * 			因为有函数依赖
 * 		+ 执行：./button_test /dev/zlj_button0
 */
