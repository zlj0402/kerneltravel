/**
 * @brief: just get screen resolution(fb_var_resolution) through /dev/fb0 -- 15/7/2025
 *
 * @description: 已经算是复习了，这次是嵌入式 linux 电子阅读器的项目; 
 * 				直接第一次学习时，没有上传 git 的习惯;
 *
 * 				+ Code:
 * 					struct fb_var_screeninfo info;
 * 					if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &info)) {
 * 					...
 *
 * 				+ fb_fd: 是 framebuffer 设备节点的 file descriptor;
 * 				+ ioctl: 根据 FBIOGET_VSCREENINFO 宏指令，去读取屏幕相关的信息;
 * 				+ FBIOGET_VSCREENINFO: 从 Framebuffer 设备获取“可变”屏幕信息（fb_var_screeninfo 结构体）;
 *
 * @output:
 * 		[root@100ask:/mnt/04_CodingAppOperatesHW/Framebuffer/01_get_screen_resolution]# ./main
 * 		Framebuffer device: /dev/fb0 opened successfully!
 * 		x_res: 1024, y_res = 600
 * 		bits_per_pixel = 32
 *
 * 		+ desc: 上面是开发板上面的屏幕信息，分辨率：横向像素点是 1024 x 纵向像素点是 600
 * 				bits_per_pixel -> 每个像素点，用 32 位(4 bytes) 来表示颜色数据;
 * 									颜色数据低 24 位是 RGB 值; 高 8 位是透明值，这个值多数屏幕用不到，至少开发板的这个配套屏幕是的;
 */
