CC = arm-linux-gnueabihf-gcc
CFLAGS = 
TARGET = main
OBJS = lcd_put_Chinese.o fontdata_8x16.o

all: target

target: $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
