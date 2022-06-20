OBJECTSDIR := $(OBJECTSDIR)/grubld/

GRUB_LOADER_OUTPUT := $(BINDIR)/grubld.bin

CFLAGS = -m32 -mregparm=1 -Wall -Wno-int-conversion -Wstrict-prototypes -march=i386 -mfpmath=387\
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -mno-mmx -mno-sse -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast \
		   -mpreferred-stack-boundary=2 -fno-toplevel-reorder -fno-toplevel-reorder -fno-tree-scev-cprop -Os

C_SRC := main printf init
ASM_SRC :=
C_DEFS := $(ARCHDEF_C)

ifeq ($(VBE_MODE), ENABLE)
	C_DEFS := -DVBE_ENABLE
endif


C_INCLUDE := -I"$(ROOTDIR)/include/std/" -I"$(ROOTDIR)/include" -I"$(LOADER_DIR)/grub_loader"

OBJECTS := $(foreach obj, $(ASM_SRC), $(OBJECTSDIR)/$(obj).o) \
           $(foreach obj, $(C_SRC), $(OBJECTSDIR)/$(obj).o)

$(OBJECTSDIR)/%.o : %.c
	mkdir -p -v $(dir $@)
	$(GCC) -c $(CFLAGS) $(C_INCLUDE) $(C_DEFS) -o $@ $^

$(OBJECTSDIR)/%.o : %.asm
	mkdir -p -v $(dir $@)
	$(ASM) $^ -f elf -o $@

.PHONY: build
build: $(OBJECTS)
	$(MKDIR) -p -v $(dir $(GRUB_LOADER_OUTPUT))
	$(LD) -s -T loader.ld -o $(GRUB_LOADER_OUTPUT) $(OBJECTS)

.PHONY: clean
clean:
	$(RM) $(GRUB_LOADER_OUTPUT) $(OBJECTS)

.PHONY: image
image:
	sudo cp $(GRUB_LOADER_OUTPUT) $(TAYHUANGOS_MOUNT_DIR)