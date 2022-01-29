IMG := ../../../tayhuangBoot.img
BOOT_OUTPUT := boot.bin
LOADER_OUTPUT := loader.bin
DD := dd
ASM := nasm
RM := rm
GCC := gcc

.PHONY: build
build:
	$(ASM) boot.asm -o $(BOOT_OUTPUT)
	$(ASM) loader.asm -o $(LOADER_OUTPUT)
	cd stage2 ; make build
	cd stage3 ; make build

.PHONY: clean
clean:
	${RM} $(BOOT_OUTPUT) $(LOADER_OUTPUT)
	cd stage2 ; make clean
	cd stage3 ; make clean

.PHONY: image
image:
	sudo ${DD} if=$(BOOT_OUTPUT) of=$(IMG) bs=512 conv=notrunc
	sudo mount -o loop $(IMG) /mnt/tayhuangBoot
	sudo cp $(LOADER_OUTPUT) /mnt/tayhuangBoot
	cd stage2 ; make image
	cd stage3 ; make image