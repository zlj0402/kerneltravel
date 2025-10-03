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
#include <button_thread.h>
#include <disp_manager.h>
#include <fonts_manager.h>
#include <encoding_manager.h>

#define STRLEN 128
#define READER_DEV "/dev/reader_button"

static inline void CommandsFault(char **argv) {

	printf("Usage: %s [-l] [-s Size] [-f font_file] [-h HZK] <text_file>\n", argv[0]);
	printf("Usage: %s -l\n", argv[0]);
}

/**
 * q -> return -1
 * n/u -> return 0
 */
static inline int HandleOperation(char cOpr) {
	
	// 按键事件发生后处理翻页
	if (cOpr == 'n') {
		ShowNextPage();
	} else if (cOpr == 'u') {
		ShowPrePage();
	} else if (cOpr == 'q') {
		return -1;
	}
	
	return 0;
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
	
	// 启动按键线程
	if (!ButtonThreadStart(READER_DEV)) {
		
		fprintf(stderr, "Failed to start button thread\n");
		return -1;
	}

	// 主循环
    while (1) {
        // 设置文件描述符集合
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        struct timeval timeout = { 0, 100000 }; // 100 ms timeout
        int ret = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);

        if (ret > 0 && FD_ISSET(STDIN_FILENO, &readfds)) {
            // 获取输入命令
            char cInput = getchar();
            if (cInput == 'n' || cInput == 'u' || cInput == 'q') {
				if (HandleOperation(cInput)) {
					
					printf("Quit display!");
					break;
				}
            }
        }
		
		char cKey;
		if (ButtonGetKey(&cKey)) {
			
			if (HandleOperation(cKey)) {
				
				printf("Quit display!\n");
				break;
			}
		}

        // 继续下一次循环
    }

    // 停止按键线程
	ButtonThreadStop();

    return 0;
}
