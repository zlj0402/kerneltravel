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

#include <disp_manager.h>
#include <fonts_manager.h>
#include <encoding_manager.h>

#define STRLEN 128

static inline void CommandsFault(char **argv) {

	printf("Usage: %s [-l] [-s Size] [-f font_file] [-h HZK] <text_file>\n", argv[0]);
	printf("Usage: %s -l\n", argv[0]);
}

int main(int argc, char* argv[]) {

	int error;
	int iOptc;
	char cOpr;
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

	while (1) {

		do {
			cOpr = getChar();
		} while ((cOpr != 'n') && (cOpr != 'u') && (cOpr != 'q'));

		if (cOpr == 'n') {
			ShowNextPage();
		}
		else if (cOpr == 'u') {
			ShowPrePage();
		}
		else {
			return 0;
		}
	}
	
	return 0;
}
