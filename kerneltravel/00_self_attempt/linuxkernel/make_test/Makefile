.PHONY: all with_make with_MAKE submake

all: with_make with_MAKE

with_make:
	@echo ">>> Top-level make with -j option"
	@make -C . submake

with_MAKE:
	@echo ">>> Top-level $(MAKE) with -j option"
	@$(MAKE) -C . submake

submake:
	@echo "Sub-make: running with MAKEFLAGS = '$(MAKEFLAGS)'"

