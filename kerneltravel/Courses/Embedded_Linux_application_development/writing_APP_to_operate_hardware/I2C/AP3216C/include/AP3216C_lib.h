#ifndef __AP3216C_LIB_H__
#define __AP3216C_LIB_H__

int ap3216c_init(int i2cbus);

int ap3216c_read_light(void);

int ap3216c_read_distance(void);

#endif
