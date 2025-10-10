CC := gcc
CFLAGS :=
TARGET := main
# 匹配当前目录下的所有 .c 文件
SRC := $(wildcard *.c)
# 把所有 .c 文件名替换成 .o，得到所有依赖的目标文件
OBJ := $(SRC:.c=.o)

all: target

target: $(OBJ)
# $^ 是自动变量，表示“所有的依赖”
	@echo "OBJ: $(OBJ)"
	$(CC) $^ -o $(TARGET)
	@echo "command: ls $(shell pwd)"
	@echo "result: $(shell ls $(shell pwd))\n"
	@echo "generate $(TARGET) successfully!\n"

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm $(OBJ) main
