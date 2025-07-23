CC = arm-linux-gnueabihf-gcc
CFLAGS = 
TARGET = main
OBJS = freetype_show_a_line.o
LIBS = -lfreetype -lm

all: target

target: $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
