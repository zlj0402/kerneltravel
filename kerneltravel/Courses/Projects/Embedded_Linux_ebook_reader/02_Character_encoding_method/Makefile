CC = gcc
CFLAGS =
TARGET = test_ansi test_utf8
SPECIAL_TARGET = test_utf8_2

all: $(TARGET) $(SPECIAL_TARGET)

$(TARGET): %: %.c
	$(CC) $(CFLAGS) -o $@ $<

$(SPECIAL_TARGET): test_ansi.c
	$(CC) -finput-charset=GBK -fexec-charset=UTF-8 -o $@ $<

clean:
	rm $(TARGET) $(SPECIAL_TARGET)
