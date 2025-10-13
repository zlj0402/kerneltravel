#include <stdio.h>
// fork(void)
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {

	int global = 0;

	pid_t cpid = fork();
	
	if (cpid < 0) {

		printf("[Error] fork failed.\n");
		return -1;
	}

	if (cpid == 0) {
		global = 100;
	}
	else if (cpid >  0) {
		printf("global = %d\n", global);
	}

	return 0;
}
