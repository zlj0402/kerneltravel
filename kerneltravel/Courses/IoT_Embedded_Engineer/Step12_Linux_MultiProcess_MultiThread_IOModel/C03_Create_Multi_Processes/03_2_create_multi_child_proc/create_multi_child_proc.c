#include <stdio.h>
// fork(void)
#include <sys/types.h>
#include <unistd.h>
// exit()
#include <stdlib.h>

#include "load.h"

int main(int argc, char **argv) {

	pid_t cpid = fork();
	if (cpid < 0) {
		
		printf("[Error] fork failed.\n");
		exit(EXIT_FAILURE);
	} else if (cpid == 0) {
		
		printf("Child A process: %d.\n", getpid());	// 只会在子进程中执行
		sleep(5);
		exit(EXIT_SUCCESS);
	} else {

		printf("Parent process tasks.\n");	// 只会在父进程中执行
		pid_t cpid2 = fork();

		if (cpid2 < 0) {

			printf("[Error] fork failed.\n");
			exit(EXIT_FAILURE);
		} else if (cpid2 == 0) {
			
			printf("Child B process: %d.\n", getpid());
			sleep(5);
			exit(EXIT_SUCCESS);
		}
		
	}

	return 0;
}
