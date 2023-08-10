include $(global-env)

include $(path-script)/env/local.mk

include $(path-script)/helper.mk

.PHONY: build

target-x86 := $(path-bin)/libs/x86/libbasec.a
target-x86_64 := $(path-bin)/libs/x86_64/libbasec.a

objects := logger.o baseio.o tostring.o

flags-c := -Wall -Wno-int-conversion -Wstrict-prototypes \
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast -mno-red-zone \
		   -fno-toplevel-reorder -fno-tree-scev-cprop -Os

include-c := -I$(path-include) -I$(path-include)/std/ -I$(path-include)/libs/ -I./third_party/include/

defs-c :=

args-c := defs-c="$(defs-c)" include-c="$(include-c)" flags-c="$(flags-c)"

args := $(args-c)

dir-x86 := dir-obj="$(path-objects)/bcl/x86/" dir-src="$(path-d)"
dir-x86_64 := dir-obj="$(path-objects)/bcl/x86_64/" dir-src="$(path-d)"

build:
	$(q)$(MAKE) $(builder-s-x86)=$(target-x86) objects="$(objects)" $(dir-x86) $(args) $(target-x86)
	$(q)$(MAKE) $(builder-s-x86_64)=$(target-x86_64) objects="$(objects)" $(dir-x86_64) $(args) $(target-x86_64)