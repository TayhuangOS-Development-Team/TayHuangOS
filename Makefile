global-env := ./script/env/global.mk

-include ./config.mk

image-sectors ?= 262144

architecture ?= x86_64
mode-boot ?= bios

filesystem ?= fat32
os-name ?= TayhuangOS

include $(global-env)
include $(path-script)/env/local.mk
include $(path-script)/helper.mk
include $(path-script)/tool.mk
include $(path-script)/util.mk
include $(path-script)/setup.mk
include $(path-script)/run.mk

.PHONY: build image __image stat_code gendoc

path-bin := $(path-e)/build/bin/
path-objects := $(path-e)/build/objects/

arg-basic := architecture=$(architecture) global-env=$(global-env) path-bin=$(path-bin) path-objects=$(path-objects)

build:
	$(q)$(MAKE) -f $(path-e)/libs/primlib/Makefile $(arg-basic) $@
	
	$(q)$(MAKE) -f $(path-e)/loader/grub/Makefile $(arg-basic) $@

image:
	$(q)$(MAKE) -f $(path-script)/image/Makefile.image global-env=$(global-env) loop=$(loop-b) start-image
	-$(q)$(MAKE) __image
	$(q)$(MAKE) -f $(path-script)/image/Makefile.image global-env=$(global-env) loop=$(loop-b) end-image

__image:
	$(q)$(MAKE) $(imager)=$(path-e)/configs/grub.cfg path=/boot/grub/ do-image
	$(q)$(MAKE) $(imager)=$(path-e)/build/bin/grub/grubld.bin path=/ do-image

stat_code:
	$(q)$(comments-stat)

gendoc:
	$(q)$(MAKE) $(doc-generator)=$(path-d)/DoxygenFile $@
	$(q)$(MAKE) -f $(path-e)/libs/primlib/Makefile $(arg-basic) $@
	$(q)$(MAKE) -f $(path-e)/loader/grub/Makefile $(arg-basic) $@