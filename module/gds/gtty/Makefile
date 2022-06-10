OBJECTSDIR := $(OBJECTSDIR)/gtty/

GTTY_OUTPUT := $(BINDIR)/gtty.mod
GTTY_SYMBOL_OUTPUT := $(BINDIR)/gtty.sym
ASM := as
RM := rm
GCC := gcc
GPP := g++
LD := ld
OBJCOPY := objcopy

CFLAGS := -m64 -Wall -Wno-int-conversion -Wstrict-prototypes -mfpmath=387 -mcmodel=large \
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast \
		   -mpreferred-stack-boundary=3 -fno-toplevel-reorder -fno-toplevel-reorder -fno-tree-scev-cprop -Os

CPPFLAGS := -m64 -Wall -Wno-int-conversion -Wstrict-prototypes -mfpmath=387 -mcmodel=large \
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast \
		   -mpreferred-stack-boundary=3 -fno-toplevel-reorder -fno-toplevel-reorder -fno-tree-scev-cprop -Os 

C_SRC := main

CPP_SRC :=

ASMFLAGS :=
ASM_SRC :=

C_DEFS := $(ARCHDEF_C)
C_INCLUDE := -I"$(GDSDIR)/gtty" -I"$(ROOTDIR)/libs/libkmod" -I"$(ROOTDIR)/include" -I"$(ROOTDIR)/include/std"

CPP_DEFS := $(ARCHDEF_C)
CPP_INCLUDE := -I"$(GDSDIR)/gtty" -I"$(ROOTDIR)/libs/libkmod" -I"$(ROOTDIR)/include" -I"$(ROOTDIR)/include/std"

LD_FLAGS := -static -L"$(BUILDDIR)/bin/" 
LIBRARIES := -lkmod 

OBJECTS := $(foreach obj, $(ASM_SRC), $(OBJECTSDIR)/$(obj).o) \
           $(foreach obj, $(C_SRC), $(OBJECTSDIR)/$(obj).o) \
		   $(foreach obj, $(CPP_SRC), $(OBJECTSDIR)/$(obj).o)

$(OBJECTSDIR)/%.o : %.cpp
	mkdir -p -v $(dir $@)
	$(GPP) -c $(CPPFLAGS) $(CPP_INCLUDE) $(CPP_DEFS) -o $@ $^

$(OBJECTSDIR)/%.o : %.c
	mkdir -p -v $(dir $@)
	$(GCC) -c $(CFLAGS) $(C_INCLUDE) $(C_DEFS) -o $@ $^

$(OBJECTSDIR)/%.o : %.S
	mkdir -p -v $(dir $@)
	$(ASM) -o $@ $^

.PHONY: build
build: $(OBJECTS)
	mkdir -p -v $(dir $(GTTY_OUTPUT))
	$(LD) $(LD_FLAGS) -T gtty.ld -o $(GTTY_OUTPUT) $^ --start-group $(LIBRARIES) --end-group
ifeq ($(MODE), debug)
	$(OBJCOPY) --only-keep-debug $(GTTY_OUTPUT) $(GTTY_SYMBOL_OUTPUT)
	$(OBJCOPY) --strip-debug $(GTTY_OUTPUT)
endif

.PHONY: clean
clean:
	$(RM) $(GTTY_OUTPUT) $(OBJECTS)

.PHONY: image
image:
	sudo cp $(GTTY_OUTPUT) $(TAYHUANGOS_MOUNT_DIR)
