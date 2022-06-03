OBJECTSDIR := $(OBJECTSDIR)/setup/

SETUP_OUTPUT := $(BINDIR)/setup.mod
SETUP_SYMBOL_OUTPUT := $(BINDIR)/setup.sym
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

FS_C_SRC := fs/fat32
C_SRC := main disk \
	$(FS_C_SRC)

CPP_SRC :=

ASMFLAGS :=
ASM_SRC :=

C_DEFS := $(ARCHDEF_C)
C_INCLUDE := -I"$(MODULEDIR)/setup" -I"$(ROOTDIR)/libs/libkmod" -I"$(ROOTDIR)/libs/libkmod/std"

CPP_DEFS := $(ARCHDEF_C)
CPP_INCLUDE := -I"$(MODULEDIR)/setup" -I"$(ROOTDIR)/libs/libkmod" -I"$(ROOTDIR)/libs/libkmod/std"

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
	mkdir -p -v $(dir $(SETUP_OUTPUT))
	$(LD) $(LD_FLAGS) -T setup.ld -o $(SETUP_OUTPUT) $^ --start-group $(LIBRARIES) --end-group
ifeq ($(MODE), debug)
	$(OBJCOPY) --only-keep-debug $(SETUP_OUTPUT) $(SETUP_SYMBOL_OUTPUT)
	$(OBJCOPY) --strip-debug $(SETUP_OUTPUT)
endif

.PHONY: clean
clean:
	$(RM) $(SETUP_OUTPUT) $(OBJECTS)

.PHONY: image
image:
	sudo cp $(SETUP_OUTPUT) $(TAYHUANGOS_MOUNT_DIR)
