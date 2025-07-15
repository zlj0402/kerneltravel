CC = arm-linux-gnueabihf-gcc
CFLAGS = 
TARGET = main
OBJ = get_screen_resolution.o

all: target

target: $(OBJ)
	$(CC) -o $(TARGET) $<

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm $(TARGET) $(OBJ)
