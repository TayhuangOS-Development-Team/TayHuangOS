OBJECTSDIR := $(OBJECTSDIR)/keyboard/

KEYBOARD_OUTPUT := $(BINDIR)/keyboard.mod
KEYBOARD_SYMBOL_OUTPUT := $(BINDIR)/keyboard.sym
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
C_INCLUDE := -I"$(DRIVERSDIR)/keyboard" -I"$(ROOTDIR)/libs/libkmod" -I"$(ROOTDIR)/include" -I"$(ROOTDIR)/include/std"

CPP_DEFS := $(ARCHDEF_C)
CPP_INCLUDE := -I"$(DRIVERSDIR)/keyboard" -I"$(ROOTDIR)/libs/libkmod" -I"$(ROOTDIR)/include" -I"$(ROOTDIR)/include/std"

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
	mkdir -p -v $(dir $(KEYBOARD_OUTPUT))
	$(LD) $(LD_FLAGS) -T keyboard.ld -o $(KEYBOARD_OUTPUT) $^ --start-group $(LIBRARIES) --end-group
ifeq ($(MODE), debug)
	$(OBJCOPY) --only-keep-debug $(KEYBOARD_OUTPUT) $(KEYBOARD_SYMBOL_OUTPUT)
	$(OBJCOPY) --strip-debug $(KEYBOARD_OUTPUT)
endif

.PHONY: clean
clean:
	$(RM) $(KEYBOARD_OUTPUT) $(OBJECTS)

.PHONY: image
image:
	sudo cp $(KEYBOARD_OUTPUT) $(TAYHUANGOS_MOUNT_DIR)
