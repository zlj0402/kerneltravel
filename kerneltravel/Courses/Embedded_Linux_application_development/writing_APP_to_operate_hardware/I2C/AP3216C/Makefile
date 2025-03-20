CC = $(BUILDROOT_CROSS_COMPILE)gcc
CFLAGS = -I ../include -c
OBJ = AP3216C_test.o i2cbusses.o AP3216C_lib.o

all: AP3216C_test

AP3216C_test: $(OBJ)
	$(CC) -o $@ $(OBJ)

%.o : %.c
	$(CC) -o $@ $(CFLAGS) $<

clean:
	rm $(OBJ) AP3216C_test
