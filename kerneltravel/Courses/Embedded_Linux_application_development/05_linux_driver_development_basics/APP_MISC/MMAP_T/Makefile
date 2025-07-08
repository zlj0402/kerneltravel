CC = gcc
CFLAGS = -c
OBJ = mmap_test.o

all: test

test: $(OBJ)
	$(CC) -o $@ $(OBJ) -static

%.o : %.c
	$(CC) -o $@ $(CFLAGS) $<

clean:
	rm $(OBJ) test
