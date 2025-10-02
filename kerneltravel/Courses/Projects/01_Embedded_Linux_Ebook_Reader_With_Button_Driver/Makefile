PROJECT_ROOT := $(PWD)
# 默认目标
all: driver src

# 编译 drivers 目录
driver:
	$(MAKE) -C drivers/button_old

# 编译 src 目录
src:
	$(MAKE) -C src PROJECT_ROOT=$(PROJECT_ROOT)

# 清理所有目标
clean:
	$(MAKE) -C drivers/button_old clean
	$(MAKE) -C src clean

.PHONY: all driver src clean
