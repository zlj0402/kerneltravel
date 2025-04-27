CC = gcc
CFLAGS = -c
OBJ = signal_test.o

all: signal_test

signal_test: $(OBJ)
	$(CC) -o $@ $(OBJ)

%.o : %.c
	$(CC) -o $@ $(CFLAGS) $<

clean:
	rm $(OBJ) signal_test
