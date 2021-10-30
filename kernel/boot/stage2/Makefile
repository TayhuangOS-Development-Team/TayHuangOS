STAGE2_OUPUT := stage2.bin
ASM := nasm
RM := rm
GCC := gcc
REALMODE_CFLAGS := -m16 -mregparm=1 -Wall -Wstrict-prototypes -march=i386 \
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -mno-mmx -mno-sse -ffreestanding -fno-stack-protector \
		   -mpreferred-stack-boundary=2 -fno-toplevel-reorder -fno-toplevel-reorder
C_SRC := stage2 print
ASM_SRC := disp stage2_entry

.PHONY: build
build:
	for asm_file in $(ASM_SRC); do \
		$(ASM) $$asm_file.asm -f elf -o $$asm_file.o; \
	done
	for c_file in $(C_SRC); do \
	    $(GCC) $(REALMODE_CFLAGS) $$c_file.c -c -o $$c_file.o; \
	done
#	$(GCC) $(REALMODE_CFLAGS) stage2.c -c -o stage2.o
#	$(GCC) $(REALMODE_CFLAGS) print.c -c -o stage2.o
	$(LD) -m elf_i386 -s -Ttext 0x0200 $(foreach asm_obj, $(ASM_SRC), $(asm_obj).o) $(foreach c_obj, $(C_SRC), $(c_obj).o) -o $(STAGE2_OUPUT)

.PHONY: clean
clean:
	${RM} stage2.o stage2_entry.o $(STAGE2_OUPUT)

.PHONY: image
image:
	sudo cp $(STAGE2_OUPUT) /mnt/flyOS