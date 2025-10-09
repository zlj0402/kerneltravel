PROJECT_ROOT := $(PWD)
SRC := src3
BUTTON_DIR_NAME := button2_$(SRC)
# 默认目标
all: driver src

# 编译 drivers 目录
driver:
	$(MAKE) -C drivers/$(BUTTON_DIR_NAME)

# 编译 src 目录
src:
	$(MAKE) -C $(SRC) PROJECT_ROOT=$(PROJECT_ROOT) SRC=$(SRC)

# 清理所有目标
clean:
	$(MAKE) -C drivers/$(BUTTON_DIR_NAME) clean
	$(MAKE) -C $(SRC) clean

.PHONY: all driver src clean
