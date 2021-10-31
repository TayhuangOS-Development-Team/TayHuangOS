IMG := ../../tayhuangOS.img
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

.PHONY: clean
clean:
	${RM} $(BOOT_OUTPUT) $(LOADER_OUTPUT)
	cd stage2 ; make clean

.PHONY: image
image:
	sudo ${DD} if=$(BOOT_OUTPUT) of=$(IMG) bs=512 conv=notrunc
	sudo mount -o loop $(IMG) /mnt/tayhuangOS
	sudo cp $(LOADER_OUTPUT) /mnt/tayhuangOS
	cd stage2 ; make image