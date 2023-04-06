global-env := ./script/env/global.mk

-include ./config.mk

image-sectors ?= 262144
offset-kernel ?= 1048576

architecture ?= x86_64
mode-boot ?= bios

filesystem ?= fat32
os-name ?= TayhuangOS

include $(global-env)
include $(path-script)/env/local.mk
include $(path-script)/tool.mk
include $(path-script)/util.mk
include $(path-script)/setup.mk

.PHONY: build

path-bin := ./build/bin/
path-objects := ./build/objects/

build:
	$(MAKE) -f $(path-e)/loader/grub/Makefile architecture=$(architecture) global-env=$(global-env) path-bin=$(path-bin) path-objects=$(path-objects) $@
	$(MAKE) -f $(path-e)/libs/ulogger/Makefile architecture=$(architecture) global-env=$(global-env) path-bin=$(path-bin) path-objects=$(path-objects) $@

image:
	$(MAKE) -f $(path-script)/image/Makefile.image global-env=$(global-env) loop=$(loop-b) start-image
	-$(MAKE) __image
	$(MAKE) -f $(path-script)/image/Makefile.image global-env=$(global-env) loop=$(loop-b) end-image

__image:
	$(MAKE) -f $(path-script)/image/Makefile.image global-env=$(global-env) target=$(path-e)/configs/grub.cfg path=/boot/grub/ do-image
	$(MAKE) -f $(path-script)/image/Makefile.image global-env=$(global-env) target=$(path-e)/build/bin/grub/grubld.bin path=/ do-image