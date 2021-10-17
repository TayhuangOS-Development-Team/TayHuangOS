IMG := ../../flyOS.img
BOOT_SOURCE := boot.asm
BOOT_OUTPUT := boot.bin
LOADER_SOURCE := loader.asm
LOADER_OUTPUT := loader.bin
DD := dd
ASM := nasm
RM := rm

.PHONY: build
build:
	$(ASM) $(BOOT_SOURCE) -o $(BOOT_OUTPUT)
	$(ASM) ${LOADER_SOURCE} -o $(LOADER_OUTPUT)

.PHONY: clean
clean:
	${RM} $(BOOT_OUTPUT) $(LOADER_OUTPUT)

.PHONY: image
image:
	sudo ${DD} if=$(BOOT_OUTPUT) of=$(IMG) bs=512 conv=notrunc
	sudo mount -o loop $(IMG) /mnt/flyOS
	sudo cp $(LOADER_OUTPUT) /mnt/flyOS
	sudo umount /mnt/flyOS