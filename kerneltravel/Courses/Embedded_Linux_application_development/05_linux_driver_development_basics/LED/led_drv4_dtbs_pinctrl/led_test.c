#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "load.h"

#define LED_DEV_NAME "zljled"


// led_test /dev/zljled on
// led_test /dev/zljled off

int main(int argc, char* argv[])
{
	int fd;
	char status;

	if (argc != 3)
	{
		printf("Usage: %s <dev> <on|off>\n", argv[0]);
		printf("	eg: %s /dev/%s on", argv[0], LED_DEV_NAME);
		printf("	eg: %s /dev/%s off", argv[0], LED_DEV_NAME);
		return -1;
	}
	
	// open
	fd = open(argv[1], O_RDWR);
	if (fd < 0)
	{
		printf("open %s failed.\n", argv[1]);
		return -1;
	}

	// write
	if (strcmp(argv[2], "on") == 0)
	{
		status = 1;
		printf("set status: %d\n", status);
	}
	else
	{
		status = 0;
		printf("set status: %d\n", status);
	}
	
	write(fd, &status, sizeof(char));
}
