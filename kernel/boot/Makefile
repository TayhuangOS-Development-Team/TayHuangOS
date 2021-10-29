IMG := ../../flyOS.img
BOOT_OUTPUT := boot.bin
LOADER_OUTPUT := loader.bin
STAGE2_OUPUT := stage2.bin
DD := dd
ASM := nasm
AS := as
RM := rm
GCC := gcc
REALMODE_CFLAGS := -m16 -mregparm=1 -Wall -Wstrict-prototypes -march=i386 \
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -mno-mmx -mno-sse -ffreestanding -fno-stack-protector \
		   -mpreferred-stack-boundary=2 -fno-toplevel-reorder -fno-toplevel-reorder

.PHONY: build
build:
	$(ASM) boot.asm -o $(BOOT_OUTPUT)
	$(ASM) loader.asm -o $(LOADER_OUTPUT)
	$(ASM) stage2_entry.asm -f elf -o stage2_entry.o
	$(GCC) $(REALMODE_CFLAGS) stage2.c -c -o stage2.o
	$(LD) -m elf_i386 -s -Ttext 0x0200 stage2.o stage2_entry.o -o $(STAGE2_OUPUT)

.PHONY: clean
clean:
	${RM} $(BOOT_OUTPUT) $(LOADER_OUTPUT) stage2.o stage2_entry.o $(STAGE2_OUPUT)

.PHONY: image
image:
	sudo ${DD} if=$(BOOT_OUTPUT) of=$(IMG) bs=512 conv=notrunc
	sudo mount -o loop $(IMG) /mnt/flyOS
	sudo cp $(LOADER_OUTPUT) /mnt/flyOS
	sudo cp $(STAGE2_OUPUT) /mnt/flyOS