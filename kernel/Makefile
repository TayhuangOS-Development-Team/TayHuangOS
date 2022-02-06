KERNEL_OUTPUT := kernel.bin
ASM := as
RM := rm
GCC := gcc
LD := ld
CFLAGS := -m64 -Wall -Wno-int-conversion -Wstrict-prototypes -mfpmath=387 -mcmodel=large \
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -mno-mmx -mno-sse -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast \
		   -mpreferred-stack-boundary=4 -fno-toplevel-reorder -fno-toplevel-reorder -fno-tree-scev-cprop -Os
ASMFLAGS := 
STD_C_SRC := std/ctype std/string
C_SRC := kernel \
		$(STD_C_SRC)
ASM_SRC := entry
C_DEFS := $(ARCHDEF_C)
C_INCLUDE := -I"../include/std/"  -I"../include/"


.PHONY: build
build:
	for asm_file in $(ASM_SRC); do \
		$(ASM) $$asm_file.S -o $$asm_file.o; \
	done
	for c_file in $(C_SRC); do \
	    $(GCC) $(CFLAGS) $(C_INCLUDE) $(C_DEFS) $$c_file.c -c -o $$c_file.o;\
	done
	$(LD) -s -T kernel.ld $(foreach asm_obj, $(ASM_SRC), $(asm_obj).o) $(foreach c_obj, $(C_SRC), $(c_obj).o) -o $(KERNEL_OUTPUT)

.PHONY: clean
clean:
	for asm_file in $(ASM_SRC); do \
		$(RM) $$asm_file.o; \
	done
	for c_file in $(C_SRC); do \
	    $(RM) $$c_file.o; \
	done
	$(RM) $(KERNEL_OUPUT)

.PHONY: image
image:
	sudo cp $(KERNEL_OUTPUT) /mnt/tayhuangOS