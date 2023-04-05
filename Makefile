global-env = ./script/env/global.mk

include ./script/env/local.mk

.PHONY: build

target := build/bin/loader/grub/grubld.bin

objects := main.o heap.o init/gdt.o libs/multiboot2.o

ld-args := flags-ld="-s -z max-page-size=0x100" script-ld=loader/grub/grub.ld

flags-c := -Wall -Wno-int-conversion -Wstrict-prototypes \
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast -mno-red-zone\
		   -fno-toplevel-reorder -fno-tree-scev-cprop -Os

c-args := defs-c="-DBITS=32" include-c="-I./include/ -I./include/std/ -I./loader/grub/ -I./third_party/include/" flags-c="$(flags-c)"

args := $(ld-args) $(c-args)

build:
	$(MAKE) -f ./script/build/Makefile.build global-env=$(global-env) mode=k target=$(target) objects="$(objects)" $(args) dir-obj="build/objects/grub/" dir-src="loader/grub/" $(target)