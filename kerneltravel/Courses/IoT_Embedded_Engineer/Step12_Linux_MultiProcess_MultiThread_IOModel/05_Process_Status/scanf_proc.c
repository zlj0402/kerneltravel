#include <stdio.h>

// TASK_RUNNING ---> TASK_INITERRUPTIBLE ---> TASK_RUNNING 
// 可以是 kill 信号，也可以是接收到输入的信号
int main() {
	
	int num = 0;
	scanf("%d", &num);
	printf("num = %d\n", num);

	return 0;
}
