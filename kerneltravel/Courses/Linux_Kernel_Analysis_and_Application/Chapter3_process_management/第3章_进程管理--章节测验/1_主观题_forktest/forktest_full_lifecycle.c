#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	pid_t cpid, w;	/* cpid -> child pid, w -> wait/waitpid return value */
	int wstatus;

	cpid = fork();
	if (cpid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (cpid == 0) {	/* Code executed by child */
		printf("Child PID is %d\n", (int)getpid());
		if (argc == 1)
			pause();	/* Wait for signals */
		_exit(atoi(argv[1]));
	} else {	/* Code executed by parent */
		do {
			w = waitpid(cpid, &wstatus, WUNTRACED | WCONTINUED);
			if (w == -1) {
				perror("waitpid");
				exit(EXIT_FAILURE);
			}

			if (WIFEXITED(wstatus)) {
				printf("child: %d existed, status=%d\n", (int)cpid, WEXITSTATUS(wstatus));
			} else if (WIFSIGNALED(wstatus)) {
				printf("killed by signal %d\n", WTERMSIG(wstatus));
			} else if (WIFSTOPPED(wstatus)) {
				printf("stopped by signal %d\n", WSTOPSIG(wstatus));
			} else if (WIFCONTINUED(wstatus)) {
				printf("continued\n");
			}
		} while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));
		exit(EXIT_SUCCESS);
	}
}
