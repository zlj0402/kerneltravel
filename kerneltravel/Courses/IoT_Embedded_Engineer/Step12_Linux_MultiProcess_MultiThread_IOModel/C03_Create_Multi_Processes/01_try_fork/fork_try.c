#include <stdio.h>
// fork(void)
#include <sys/types.h>
#include <unistd.h>
// getpid(void)
#include <sys/types.h>
#include <unistd.h>

#include "load.h"

int main(int argc, char** argv) {
	
	pid_t fork_res = fork();

	if (fork_res == -1) {

		printf("[Error] fork()\n");
	}

	printf("Hello Fork()\n");
	printf("fork_res = %d, pid = %d\n", fork_res, getpid());

	return 0;
}
