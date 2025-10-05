#include <queue.h>
#include <button_thread.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
// pollfd
#include <poll.h>

// 按键线程相关全局
static int g_iDevFd = -1;
static pthread_t g_tKeyThread;
static volatile bool g_bRun = true;

// 驱动那边：jiffies（取模后的）[31,8] + gpio num [7, 4] + 按键值 [3, 0]
// 这边直接将值放入数组
static void *ButtonThread(void *arg) {

    struct pollfd t_pfd = { .fd = g_iDevFd, .events = POLLIN };

    while (g_bRun) {
		
        int iRet = poll(&t_pfd, 1, 1000); // 1s超时，方便退出
        if (iRet > 0 && (t_pfd.revents & POLLIN)) {
			
            int iVal;
            ssize_t n = read(g_iDevFd, &iVal, sizeof(iVal));
            if (n == sizeof(iVal)) {
                EnqueueKey(iVal);
            }
        }
    }
    return NULL;
}

/**
 * @brief: 启动按键线程
 * @retval: 0 -> init successfully
 * @retval: 1 -> error occured
 */
int ButtonInit(const char *pacDev) {
	
    g_iDevFd = open(pacDev, O_RDONLY);
    if (g_iDevFd < 0) {
		
        perror("open");
        return -1;
    }
    g_bRun = true;
    if (pthread_create(&g_tKeyThread, NULL, ButtonThread, NULL) != 0) {
		
        perror("pthread_create");
        close(g_iDevFd);
        return -1;
    }
    return 0;
}

// 停止按键线程
void ButtonExit(void) {
	
    g_bRun = false;
	if (g_iDevFd >= 0) {
		
		close(g_iDevFd);	// 可以立即唤醒 poll() 退出，不用再等上 1000 ms;
		g_iDevFd = -1;
	}
    pthread_join(g_tKeyThread, NULL);
}

// 获取按键（非阻塞）
int ButtonGetKey(char *pcVal) {
    return DequeueKey(pcVal);
}