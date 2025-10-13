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
		printf("Child process tasks.\n");	// 只会在子进程中执行
	} else {
		printf("Parent process tasks.\n");	// 只会在父进程中执行
	}

	printf("Parent and Child processes tasks.\n");	// 父子进程做相同的任务

	return 0;
}
