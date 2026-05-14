CROSSCOMPILE := arm-linux-gnueabihf-

SRC ?= $(notdir $(CURDIR))
CFLAGS 	:= -Wall -O2 -c
CFLAGS  += -I$(PROJECT_ROOT)/$(SRC)/include

# for 100ask imx6ull
#CFLAGS += -I /home/book/100ask_imx6ull-sdk/ToolChain/arm-buildroot-linux-gnueabihf_sdk-buildroot/arm-buildroot-linux-gnueabihf/sysroot/usr/include/freetype2

LDFLAGS := -lm -lfreetype -lpthread

CC 	:= $(CROSSCOMPILE)gcc

OBJS := main.o \
			display/fb.o                  \
			display/disp_manager.o        \
			encoding/ascii.o              \
			encoding/utf16_le.o           \
			encoding/utf16_be.o           \
			encoding/utf8.o               \
			encoding/encoding_manager.o   \
			fonts/ascii.o                 \
			fonts/gbk.o                   \
			fonts/freetype.o              \
			fonts/fonts_manager.o		  \
			draw/draw.o                   \
			utils/utils.o				  \
			threads/button_thread.o		  \
			threads/queue.o

all: $(OBJS)
	$(CC) $(LDFLAGS) -o show_file $^

clean:
	rm -f show_file
	rm -f $(OBJS)

%.o:%.c
	$(CC) $(CFLAGS) -o $@ $<


