include $(global-env)

include $(path-script)/env/local.mk
include $(path-script)/helper.mk

.PHONY: build

target := $(path-bin)/kernel/tayKernel.bin

objects := main.o

subdirs :=

include $(foreach subdir, $(subdirs), $(path-d)/$(subdir)/include.mk)

flags-ld := -s -z max-page-size=0x100

args-ld := flags-ld="$(flags-ld)" script-ld="$(path-d)/kernel.ld"

flags-c := -Wall -Wno-int-conversion -Wstrict-prototypes \
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast -mno-red-zone \
		   -fno-toplevel-reorder -fno-tree-scev-cprop -Os

include-c := -I$(path-include) -I$(path-include)/std/ -I$(path-d) -I./third_party/include/

defs-c :=

args-c := defs-c="$(defs-c)" include-c="$(include-c)" flags-c="$(flags-c)"

flags-asm :=

args-asm := flags-asm="$(flags-asm)"

args := $(args-ld) $(args-c) $(args-asm) libraries=basec

dir := dir-obj="$(path-objects)/kernel/" dir-src="$(path-d)"

build:
	$(q)$(MAKE) $(builder-k-x86_64)=$(target) objects="$(objects)" $(dir) $(args) $(target)