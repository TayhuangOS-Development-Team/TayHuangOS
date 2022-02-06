STAGE3_OUPUT := stage3.bin
ASM := nasm
RM := rm
GCC := gcc
LD := ld
CFLAGS = -m32 -mregparm=1 -Wall -Wno-int-conversion -Wstrict-prototypes -march=i386 -mfpmath=387\
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -mno-mmx -mno-sse -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast \
		   -mpreferred-stack-boundary=2 -fno-toplevel-reorder -fno-toplevel-reorder -fno-tree-scev-cprop #-Os
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

.PHONY: build
build:
	for asm_file in $(ASM_SRC); do \
		$(ASM) $$asm_file.asm -f elf -o $$asm_file.o; \
	done
	for c_file in $(C_SRC); do \
	    $(GCC) $(CFLAGS) $(C_INCLUDE) $(C_DEFS) $$c_file.c -c -o $$c_file.o;\
	done
	$(LD) -m elf_i386 -s -Ttext 0x1000 $(foreach asm_obj, $(ASM_SRC), $(asm_obj).o) $(foreach c_obj, $(C_SRC), $(c_obj).o) -o $(STAGE3_OUPUT)

.PHONY: clean
clean:
	for asm_file in $(ASM_SRC); do \
		$(RM) $$asm_file.o; \
	done
	for c_file in $(C_SRC); do \
	    $(RM) $$c_file.o; \
	done
	$(RM) $(STAGE3_OUPUT)

.PHONY: image
image:
	sudo cp $(STAGE3_OUPUT) $(IMG_MNT)