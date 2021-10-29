LD := ld
GCC := gcc
ASM := nasm
RM := rm
KERNEL_OUTPUT := kernel.bin

.PHONY: build
build:
	cd boot ; make build
	$(ASM) entry.asm -f elf -o entry.o
	$(GCC) -m32 kernel.c -c -o kernel.o
	$(LD) -S -m elf_i386 -s -Ttext 0x0200 kernel.o entry.o -o $(KERNEL_OUTPUT)

.PHONY: clean
clean:
	cd boot ; make clean
	${RM} $(KERNEL_OUTPUT) entry.o kernel.o

.PHONY: image
image:
	cd boot ; make image
	sudo cp $(KERNEL_OUTPUT) /mnt/flyOS