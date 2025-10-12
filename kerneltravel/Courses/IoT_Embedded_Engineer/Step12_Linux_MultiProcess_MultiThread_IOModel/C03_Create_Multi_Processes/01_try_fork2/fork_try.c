#include <stdio.h>
// fork(void) 
#include <sys/types.h>
	// write fputs
#include <unistd.h>

#include "load.h"

int main(int argc, char** argv) {
	
	write(1, "HelloWorld", 5);
	// fputs("HelloWorld", stdout);
	fputs("HelloWorld\n", stdout);

	pid_t fork_res = fork();

	if (fork_res == -1) {

		printf("[Error] fork()\n");
	}

	// printf("Hello Fork()\n");
	// printf("fork_res = %d, pid = %d\n", fork_res, getpid());

	return 0;
}
