CC = arm-linux-gnueabihf-gcc
CFLAGS = 
TARGET = main
OBJ = draw_a_line.o

all: target

target: $(OBJ)
	$(CC) -o $(TARGET) $<

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm $(TARGET) $(OBJ)
