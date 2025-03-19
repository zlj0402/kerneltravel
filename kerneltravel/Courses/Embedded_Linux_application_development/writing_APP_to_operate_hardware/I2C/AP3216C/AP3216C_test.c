#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "AP3216C_lib.h"

int main(int argc, char* argv[])
{
	if (!(argc == 3 || argc == 4))
	{
		printf("Usage:\n");
		printf("read AP3216C light: %s SMBUS_NO. -l\n", argv[0]);
		printf("read AP3216C distance: %s SMBUS_NO. -d\n", argv[0]);
		return -1;
	}

	int i2cbus = atoi(argv[1]);
	printf("get i2cbus: %d\n", i2cbus);
	
	int err = ap3216c_init(i2cbus);
	if (err < 0)
	{
		printf("ap3216c_init error: %d\n", err);
		return err;
	}

	if (argv[2][1] == 'l')
	{
		while(1)
		{
			int light = ap3216c_read_light();
			printf("read light: %d 0x%x\n", light, light);
			sleep(3);
		}
	}
	else 
	{
		while(1)
		{
			int distance = ap3216c_read_distance();
			printf("read distance: %d, 0x%x\n", distance, distance);
			sleep(3);
		}
	}

	return 0;
}

