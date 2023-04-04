global-env = ./script/env/global.mk

include ./script/env/local.mk

.PHONY: build

build:
	$(MAKE) -f ./script/build/Makefile.build global-env=$(global-env)