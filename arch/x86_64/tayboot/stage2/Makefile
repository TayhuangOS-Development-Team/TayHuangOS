STAGE2_OUPUT := stage2.bin
ASM := nasm
RM := rm
GCC := gcc
LD := ld
REALMODE_CFLAGS := -m16 -mregparm=1 -Wall -Wno-int-conversion -Wstrict-prototypes -march=i386 -mfpmath=387\
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -mno-mmx -mno-sse -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast \
		   -mpreferred-stack-boundary=2 -fno-toplevel-reorder -fno-toplevel-reorder -Os
DRIVER_C_SRC := ./drivers/video/video_driver ./drivers/memory/memory_driver ./drivers/disk/disk_driver \
				./drivers/keyboard/keyboard_driver \
			    ./drivers/driver ./drivers/drivers ./drivers/devices \
				./drivers/disk/filesystems/fs_fat16 ./drivers/disk/filesystem
STDLIB_C_SRC := ./std/string ./std/ctype printf scanf
PM_C_SRC := ./pm/entry ./pm/a20 ./pm/load_stage3 ./pm/vesa
CONSOLE_C_SRC := ./console/console ./console/commands
C_SRC := stage2_main intcall buffer heap tools file \
	$(DRIVER_C_SRC) \
	$(STDLIB_C_SRC) \
	$(PM_C_SRC) \
    $(CONSOLE_C_SRC)
C_INCLUDE := -I "../include/std/" -I"../include/tayhuang/"
C_DEFS := $(ARCHDEF_C) \
	      -DREALMODE_CONSOLE_MAJOR_VERSION=1 -DREALMODE_CONSOLE_MINOR_VERSION=0 -DREALMODE_CONSOLE_REVISION_VERSION=0 -DREALMODE_CONSOLE_BUILD_VERSION=3 \
		  -DREALMODE -DGOTO_OS_AUTO -DENABLE_GRAPHIC_BEFORE_GOTO_OS
ASM_SRC := stage2_entry io ./pm/jumping
SPLASHES := splashes.txt

.PHONY: build
build:
	for asm_file in $(ASM_SRC); do \
		$(ASM) $$asm_file.asm -f elf -o $$asm_file.o; \
	done
	for c_file in $(C_SRC); do \
	    $(GCC) $(REALMODE_CFLAGS) $(C_INCLUDE) $(C_DEFS) $$c_file.c -c -o $$c_file.o;\
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
	$(RM) $(STAGE2_OUPUT)

.PHONY: image
image:
	sudo cp $(STAGE2_OUPUT) $(IMG_MNT)
	sudo cp $(SPLASHES) $(IMG_MNT)