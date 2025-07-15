/**
 * @brief: draw a line on screen with mmap -- 15/7/2025
 *
 * @description: 在得到屏幕信息的基础上 -- 上一节
 * 			+ mmap 映射 /dev/fb0 设备节点对应的 Framebuffer，得到映射内存的起始虚拟地址;
 * 				fb_base = mmap(NULL, screen_st, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
 * 				+ 第一个参数: 我们想要的映射的内存空间的起始虚拟地址;
 * 				+ 第二个参数: 屏幕显示数据的大小: 像素点数(xres * yres) * 每个像素值(2 3 个byte的RGB值)
 * 				+ 第三个参数: 映射页的权限申请，之后我们要读写的话就是: PROT_READ | PROT_WRITE
 * 				+ 第四个参数: 对内存的修改时，是原内存直接修改(MAP_SHARED)，还是在copy的那份上的修改(MAP_PRIVATE);
 * 				+ 第五个参数: file descriptor
 * 				+ 第六个参数: 相对于映射的内存地址的偏移，需要是 PAGE_SHIFT 的倍数;
 * 			+ 根据坐标，和像素占据的位数，赋值一个 RGB 值;
 *
 * @steps:
 * 		在得到屏幕信息的基础上后，要做的也就是上面 @description 中的两步;
 *
 * @phen.:
 * 		在屏幕中心往右，划了一条线，100 个像素长度;
 */
