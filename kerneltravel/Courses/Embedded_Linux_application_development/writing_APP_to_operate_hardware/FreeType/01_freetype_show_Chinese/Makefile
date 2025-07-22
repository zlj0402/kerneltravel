CC = arm-linux-gnueabihf-gcc
CFLAGS = 
TARGET = main
OBJS = freetype_show_Chinese.o fontdata_8x16.o
LIBS = -lfreetype

all: target

target: $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
