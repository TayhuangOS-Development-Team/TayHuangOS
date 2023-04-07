include $(global-env)

include $(path-script)/env/local.mk

.PHONY: build

target := $(path-bin)/grub/grubld.bin

objects := main.o heap.o

subdirs := init/ libs/

include $(foreach subdir, $(subdirs), $(path-d)/$(subdir)/include.mk)

flags-ld := -s -z max-page-size=0x100

args-ld := flags-ld="$(flags-ld)" script-ld="$(path-d)/grub.ld"

flags-c := -Wall -Wno-int-conversion -Wstrict-prototypes \
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast -mno-red-zone \
		   -fno-toplevel-reorder -fno-tree-scev-cprop -Os

include-c := -I$(path-include) -I$(path-include)/std/ -I$(path-include)/libs/ -I$(path-d) -I./third_party/include/

defs-c :=

args-c := defs-c="$(defs-c)" include-c="$(include-c)" flags-c="$(flags-c)"

args := $(args-ld) $(args-c) libraries=primlib

dir := dir-obj="$(path-objects)/grub/" dir-src="$(path-d)"

build:
	$(MAKE) -f $(path-script)/build/Makefile.build global-env=$(global-env) architecture=x86 mode=k target=$(target) objects="$(objects)" $(dir) $(args) $(target)