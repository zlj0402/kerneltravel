#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int a;
int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("Usage: %s <number>\n", argv[0]);
		return -1;
	}
	a = strtol(argv[1], NULL, 0);
	printf("a's address = %p, a's value = %d\n", &a, a);
	while (1) 
	{
		sleep(10);
	}

	return 0;
}
