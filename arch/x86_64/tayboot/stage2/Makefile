OBJECTSDIR := $(OBJECTSDIR)/stage2/

STAGE2_OUTPUT := $(BINDIR)/stage2.bin
ASM := nasm
RM := rm
GCC := gcc
LD := ld
CFLAGS := -m16 -mregparm=1 -Wall -Wno-int-conversion -Wstrict-prototypes -march=i386 -mfpmath=387\
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -mno-mmx -mno-sse -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast \
		   -mpreferred-stack-boundary=2 -fno-toplevel-reorder -fno-toplevel-reorder -Os
DRIVER_C_SRC := ./drivers/video/video_driver ./drivers/memory/memory_driver ./drivers/disk/disk_driver \
				./drivers/keyboard/keyboard_driver \
			    ./drivers/driver ./drivers/drivers ./drivers/devices \
				./drivers/disk/filesystems/fs_fat16 ./drivers/disk/filesystem
STDLIB_C_SRC := ./std/string ./std/ctype printf scanf
PM_C_SRC := ./pm/entry ./pm/a20 ./pm/load_stage3 ./pm/vesa
C_SRC := stage2_main intcall buffer heap tools file \
	$(DRIVER_C_SRC) \
	$(STDLIB_C_SRC) \
	$(PM_C_SRC)
C_INCLUDE := -I "../include/std/" -I"../include/tayhuang/"
C_DEFS := $(ARCHDEF_C) \
		  -DREALMODE #-DENABLE_GRAPHIC_BEFORE_GOTO_OS
ASM_SRC := stage2_entry io ./pm/jumping
SPLASHES := splashes.txt

OBJECTS := $(foreach obj, $(ASM_SRC), $(OBJECTSDIR)/$(obj).o) \
           $(foreach obj, $(C_SRC), $(OBJECTSDIR)/$(obj).o)

$(OBJECTSDIR)/%.o : %.c
	mkdir -p -v $(dir $@)
	$(GCC) -c $(CFLAGS) $(C_INCLUDE) $(C_DEFS) -o $@ $^

$(OBJECTSDIR)/%.o : %.asm
	mkdir -p -v $(dir $@)
	$(ASM) $^ -f elf -o $@

.PHONY: build
build: $(OBJECTS)
	$(LD) -m elf_i386 -s -Ttext 0x0600 -o $(STAGE2_OUTPUT) $^

.PHONY: clean
clean:
	$(RM) $(STAGE2_OUTPUT) $(OBJECTS)

.PHONY: image
image:
	sudo cp $(STAGE2_OUTPUT) $(IMG_MNT)
	sudo cp $(SPLASHES) $(IMG_MNT)