OBJECTSDIR := $(OBJECTSDIR)/stage3/

STAGE3_OUTPUT := $(BINDIR)/stage3.bin
ASM := nasm
RM := rm
GCC := gcc
LD := ld
CFLAGS = -m32 -mregparm=1 -Wall -Wno-int-conversion -Wstrict-prototypes -march=i386 -mfpmath=387\
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -mno-mmx -mno-sse -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast \
		   -mpreferred-stack-boundary=2 -fno-toplevel-reorder -fno-toplevel-reorder -fno-tree-scev-cprop -Os
STD_C_SRC := std/ctype std/string
LM_C_SRC := lm/entry lm/load_kernel lm/setup_longmode lm/setup_paging
FS_SRC := fs/fat32
C_SRC := stage3_main video printf int_handlers init memory disk \
		$(STD_C_SRC) \
		$(LM_C_SRC) \
		$(FS_SRC)
ASM_SRC := stage3_entry _int_handlers ./lm/lm_operators
C_DEFS := $(ARCHDEF_C)
C_INCLUDE := -I"../include/std/" -I"../include/tayhuang/" -I"../../../../include/"

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
	$(LD) -m elf_i386 -s -Ttext 0x1000 -o $(STAGE3_OUTPUT) $(OBJECTS)

.PHONY: clean
clean:
	$(RM) $(STAGE3_OUTPUT) $(OBJECTS)

.PHONY: image
image:
	sudo cp $(STAGE3_OUTPUT) $(IMG_MNT)