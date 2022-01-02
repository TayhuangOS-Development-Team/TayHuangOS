STAGE2_OUPUT := stage2.bin
ASM := nasm
RM := rm
GCC := gcc
REALMODE_CFLAGS := -m16 -mregparm=1 -Wall -Wno-int-conversion -Wstrict-prototypes -march=i386 \
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -mno-mmx -mno-sse -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast \
		   -mpreferred-stack-boundary=2 -fno-toplevel-reorder -fno-toplevel-reorder -Os
C_SRC := stage2_main printf intcall heap \
	./drivers/vedio/vedio_driver ./drivers/memory/memory_driver ./drivers/disk/disk_driver ./drivers/disk/filesystem \
	./drivers/disk/filesystems/fs_fat16 \
	./drivers/driver ./drivers/drivers ./drivers/devices \
	./std/string
C_INCLUDE := -I "./std/"
ASM_SRC := stage2_entry io
SPLASHES := splashes.txt

.PHONY: build
build:
	for asm_file in $(ASM_SRC); do \
		$(ASM) $$asm_file.asm -f elf -o $$asm_file.o; \
	done
	for c_file in $(C_SRC); do \
	    $(GCC) $(REALMODE_CFLAGS) $(C_INCLUDE) $$c_file.c -c -o $$c_file.o;\
	done
	$(LD) -m elf_i386 -s -Ttext 0x0600 $(foreach asm_obj, $(ASM_SRC), $(asm_obj).o) $(foreach c_obj, $(C_SRC), $(c_obj).o) -o $(STAGE2_OUPUT)

.PHONY: clean
clean:
	for asm_file in $(ASM_SRC); do \
		$(RM) $$asm_file.o; \
	done
	for c_file in $(C_SRC); do \
	    $(RM) $$c_file.o; \
	done

.PHONY: image
image:
	sudo cp $(STAGE2_OUPUT) /mnt/tayhuangOS
	sudo cp $(SPLASHES) /mnt/tayhuangOS