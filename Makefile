global-env = ./script/env/global.mk

include $(global-env)
include $(path-script)/env/local.mk

.PHONY: build

path-bin := ./build/bin/
path-objects := ./build/objects/

build:
	$(MAKE) -f ./loader/grub/Makefile global-env=$(global-env) path-bin=$(path-bin) path-objects=$(path-objects) $@