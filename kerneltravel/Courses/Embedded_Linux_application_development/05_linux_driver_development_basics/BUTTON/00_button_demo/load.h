/*
 * @brief: button drv demo
 * 
 * @description:
 * 		+ 没有用 platform_device 和 platform_driver
 * 		+ 只是简单的注册了个 file_operations 驱动程序
 *
 * @steps:
 * 		+ 执行: make
 * 		+ 将 board_xxx.ko button_drv.ko button_test 一起，拷贝到 ubuntu 和开发板共享的目录;
 * 		+ 先 insmod button_drv.ko，后 insmod board_xxx.ko;
 * 			因为有函数依赖
 * 		+ 执行：./button_test /dev/zlj_button0
 */