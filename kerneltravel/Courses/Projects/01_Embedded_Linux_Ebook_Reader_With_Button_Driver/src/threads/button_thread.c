#include <button_thread.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>

// 按键线程相关全局
static int g_iDevFd = -1;
static volatile bool g_bPollThreadRunning = false;
static pthread_t g_tPollThread;

// 信号安全按键存储
static volatile sig_atomic_t g_tKeyPending = 0;
static volatile sig_atomic_t g_tKeyVal = 0;

// 信号处理函数
static void SigFunc(int sig) {
	
    int iVal;
    ssize_t n = read(g_iDevFd, &iVal, sizeof(iVal));
    if (n == sizeof(iVal)) {
		
        int val = iVal & 0x00ff;
        if (val == 1) g_tKeyVal = 'u';   // 上一页
        else if (val == 2) g_tKeyVal = 'n'; // 下一页
        else g_tKeyVal = 0;
        g_tKeyPending = 1;
    }
}

// 打开按键设备并设置异步信号
static int OpenButtonDev(const char *dev_node) {
	
    g_iDevFd = open(dev_node, O_RDWR);
    if (g_iDevFd == -1) {
		
        perror("Open button device");
        return -1;
    }

    signal(SIGIO, SigFunc);
    fcntl(g_iDevFd, F_SETOWN, getpid());
    int flags = fcntl(g_iDevFd, F_GETFL);
    fcntl(g_iDevFd, F_SETFL, flags | FASYNC);

    return 0;
}

// 按键线程函数
static void* ButtonThreadFunc(void *arg) {
	
    const char *dev_node = (const char*)arg;
    if (OpenButtonDev(dev_node)) {
		
        fprintf(stderr, "Failed to open button device node\n");
        return NULL;
    }

    while (g_bPollThreadRunning) {
        sleep(10);
    }

    close(g_iDevFd);
    g_iDevFd = -1;
    return NULL;
}

// 外部接口
bool ButtonThreadStart(const char *dev_node) {
	
    if (g_bPollThreadRunning) return false; // 已经启动

    g_bPollThreadRunning = true;
    if (pthread_create(&g_tPollThread, NULL, ButtonThreadFunc, (void*)dev_node) != 0) {
		
        perror("Failed to create button thread");
        g_bPollThreadRunning = false;
        return false;
    }
    return true;
}

void ButtonThreadStop(void) {
	
    if (!g_bPollThreadRunning) return;

    g_bPollThreadRunning = false;
    pthread_join(g_tPollThread, NULL);
}

int ButtonGetKey(char *val) {
	
    if (g_tKeyPending && g_tKeyVal) {
		
        *val = g_tKeyVal;
        g_tKeyPending = 0;
        g_tKeyVal = 0;
        return 1;
    }
    return 0;
}
