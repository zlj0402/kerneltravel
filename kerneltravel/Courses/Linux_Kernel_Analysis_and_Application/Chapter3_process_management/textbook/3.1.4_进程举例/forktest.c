#include <sys/types.h>	/* 提供类型pid_t的定义, 在PC上与int型相同 */
#include <unistd.h> 	/* 提供系统调用的定义 */
#include <stdio.h>	/* 提供基本输入输出函数, 如printf */

void do_something(long t);

int main()
{
	pid_t pid;

	/* 此时仅有一个进程 */
	printf("PID before fork(): %d\n", getpid());
	pid = fork();

	/* 此时已经有两个进程在同时运行 */
	pid_t npid = getpid();
	if (pid < 0)
		perror("fork error\n");
	else if (pid == 0) { /* pid == 0 表示子进程 */
		while (1) {
			printf("I am child process, PID is %d, reachable? pid: %d\n", npid, pid);
			do_something(10000000);
		}
	} else if (pid >= 0) { /* pid > 0 表示父进程 */
		while (1) {
			printf("I am father process, PID is %d, reachable? pid: %d\n", npid, pid);
			do_something(10000000);
		}
	}

	return 0;
}

void do_something(long t)
{
	/* 遍历一遍t, 就会结束遍历 */
	int i = 0;
	for (i = 0; i < t; i++)
		for (i = 0; i < t; i++)
			for (i = 0; i < t; i++)
				;
}
