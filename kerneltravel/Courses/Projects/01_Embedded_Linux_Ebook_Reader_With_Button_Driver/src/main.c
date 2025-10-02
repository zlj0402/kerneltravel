// bool
#include <stdbool.h>
// printf
#include <stdio.h>
// strcpy strncpy
#include <string.h>
// getopt
#include <unistd.h>
// strtoul
#include <stdlib.h>
// static pthread_mutex_t
#include <pthread.h>
// open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// signal
#include <signal.h>

#include <draw.h>
#include <config.h>
#include <disp_manager.h>
#include <fonts_manager.h>
#include <encoding_manager.h>

#define STRLEN 128
#define READER_DEV "/dev/reader_button"

static char g_cOpr;
static int g_iDevFd;
static pthread_mutex_t g_tOprMutex = PTHREAD_MUTEX_INITIALIZER;  // 互斥锁
static bool g_bPollThreadRunning = true;  // 新增的变量，用于控制线程运行

// 用于信号安全的标志
volatile sig_atomic_t g_key_pending = 0;
volatile sig_atomic_t g_key_val = 0; // 存储按键的值

bool g_bKeyEvent = false; // 按键事件标志

// 信号处理函数
static void SigFunc(int sig) {
    int iVal;
    ssize_t n = read(g_iDevFd, &iVal, sizeof(iVal)); // read 是 async-signal-safe
    if (n == sizeof(iVal)) {
        int val = iVal & 0x00ff;
        if (val == 1) g_key_val = 'u';   // 上一页
        else if (val == 2) g_key_val = 'n';  // 下一页
        else g_key_val = 0;  // 其他无效按键
        g_key_pending = 1;  // 标记有按键待处理
    }
}

static int OpenButtonDev(char *pcButtonDev) {

	g_iDevFd = open(pcButtonDev, O_RDWR);
	if (g_iDevFd == -1) {

		printf("can not open file %s\n", pcButtonDev);
		return -1;
	}

	signal(SIGIO, SigFunc);
	fcntl(g_iDevFd, F_SETOWN, getpid());
	int iOflags = fcntl(g_iDevFd, F_GETFL);
	fcntl(g_iDevFd, F_SETFL, iOflags | FASYNC);
	
	return 0;
}

void* ReadButton(void *arg) {

	if (OpenButtonDev((char*)arg)) {
		
		printf("Open button device node error\n");
		return NULL;
	}
	
	while(g_bPollThreadRunning) {

		DBG_PRINTF(" -- \n");
		sleep(10);
	}

	close(g_iDevFd);

	return NULL;
}

static inline void CommandsFault(char **argv) {

	printf("Usage: %s [-l] [-s Size] [-f font_file] [-h HZK] <text_file>\n", argv[0]);
	printf("Usage: %s -l\n", argv[0]);
}

int main(int argc, char* argv[]) {

	int error;
	int iOptc;
	unsigned int dwFontSize = 16;
	char acDisplay[STRLEN];
	char acHzkFile[STRLEN];
	char acFreetypeFile[STRLEN];
	char acTextFile[STRLEN];

	strcpy(acDisplay, "fb");

	acHzkFile[0] = '\0';
	acFreetypeFile[0] = '\0';
	acTextFile[0] = '\0';

	bool bList = false;

	while ((iOptc = getopt(argc, argv, "ls:f:h:")) != -1) {

		switch(iOptc) {

			case 'l': {

				bList = true;
				break;
			}
			case 's': {

				dwFontSize = strtoul(optarg, NULL, 0);
				break;
			}
			case 'f': {

				strncpy(acFreetypeFile, optarg, STRLEN);
				// 以防 optarg 长度 > STRLEN
				acFreetypeFile[STRLEN - 1] = '\0';
				break;
			}
			case 'h': {

				strncpy(acHzkFile, optarg, STRLEN);
				acHzkFile[STRLEN - 1] = '\0';
				break;
			}
			default: {
			
				CommandsFault(argv);
				return -1;
			}
		}
	}

	/**
	 * !bList 的判断是必要的；
	 * 想表示，Command line 没有带 -l 时，必须带上一个 Operands;
	 * PS: 选项扫描结束后，optind 停在第一个 operand 的下标
	 */
	if (!bList && (optind >= argc)) {
		
		CommandsFault(argv);
		return -1;
	}

	error = DisplayInit();
	if (error) {

		printf("DisplayInit error!\n");
		return -1;
	}

	// 先 Init Font 再 Init Encoding
	error = FontsInit();
	if (error) {

		printf("FontsInit error!\n");
		return -1;
	}

	error = EncodingInit();
	if (error) {

		printf("EncodingInit error!\n");
		return -1;
	}

	if (bList) {

		printf("supported display:\n");
		ShowDispOpr();

		printf("supported font:\n");
		ShowFontOpr();

		printf("supported encoding:\n");
		ShowEncodingOpr();
	}

	strncpy(acTextFile, argv[optind], STRLEN);
	acTextFile[STRLEN - 1] = '\0';

	error = OpenTextFile(acTextFile);
	if (error) {

		printf("OpenTextFile error!\n");
		return -1;
	}

	error = SetTextDetail(acHzkFile, acFreetypeFile, dwFontSize);
	if (error) {

		printf("SetTextDetail error!\n");
		return -1;
	}
	
	error = SelectAndInitDisplay(acDisplay);
	if (error) {

		printf("SelectAndInitDispaly error!\n");
		return -1;
	}

	error = ShowNextPage();
	if (error) {

		printf("Error to show first page\n");
		return -1;
	}
	
    // 启动按键监听线程
    pthread_t tPollThread;
    if (pthread_create(&tPollThread, NULL, ReadButton, READER_DEV) != 0) {
        perror("Failed to create poll thread");
        close(g_iDevFd);
        return -1;
    }

	// 主循环
    while (1) {
        // 设置文件描述符集合
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        struct timeval timeout = { 0, 100000 }; // 设置适当的超时
        int ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);

        if (ret > 0 && FD_ISSET(STDIN_FILENO, &readfds)) {
            // 获取输入命令
            char cInput = getchar();
            if (cInput == 'n' || cInput == 'u' || cInput == 'q') {
                pthread_mutex_lock(&g_tOprMutex);  	// 只在修改 g_cOpr 时加锁
                g_cOpr = cInput;
                pthread_mutex_unlock(&g_tOprMutex);  // 释放锁

                if (g_cOpr == 'n') {
                    ShowNextPage();
                } else if (g_cOpr == 'u') {
                    ShowPrePage();
                } else {
                    // 设置标志位停止按键监听线程
                    g_bPollThreadRunning = false;
                    break;
                }
            }
        }

        // 非阻塞地检查并处理按键事件
        if (g_key_pending) {
            pthread_mutex_lock(&g_tOprMutex);
            if (g_key_val) {
                g_cOpr = (char)g_key_val;
            }
            pthread_mutex_unlock(&g_tOprMutex);

            // 清除 pending（volatile sig_atomic_t）
            g_key_pending = 0;
            g_key_val = 0;

            // 按键事件发生后处理翻页
            pthread_mutex_lock(&g_tOprMutex);
            if (g_cOpr == 'n') {
                ShowNextPage();
            } else if (g_cOpr == 'u') {
                ShowPrePage();
            } else if (g_cOpr == 'q') {
                g_bPollThreadRunning = false;
                pthread_mutex_unlock(&g_tOprMutex);
                break;
            }
            pthread_mutex_unlock(&g_tOprMutex);
        }

        // 继续下一次循环
    }

    // 等待按键线程结束
    pthread_join(tPollThread, NULL);

    return 0;
}
