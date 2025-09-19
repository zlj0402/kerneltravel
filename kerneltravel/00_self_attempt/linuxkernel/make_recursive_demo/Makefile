.PHONY: all with_make with_MAKE

all: with_make with_MAKE

with_make:
	@echo ">>> Recursive build using plain 'make'"
	@make -C subdir

with_MAKE:
	@echo ">>> Recursive build using '$(MAKE)'"
	@$(MAKE) -C subdir

