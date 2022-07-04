#
# SPDX-License-Identifier: GPL-3.0-only
# -------------------------------*-TayhuangOS-*-----------------------------------
# 
#    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
# 
# --------------------------------------------------------------------------------
# 
# 作者: theflysong
# 
# Makefile
# 
# Makefile
# 
#



OBJECTSDIR := $(OBJECTSDIR)/kernel/

KERNEL_OUTPUT := $(BINDIR)/kernel.bin
KERNEL_SYMBOL_OUTPUT := $(BINDIR)/kernel.sym
ASM := as
RM := rm
GCC := gcc
GPP := g++
LD := ld
OBJCOPY := objcopy

CFLAGS := -m64 -Wall -Wno-int-conversion -Wstrict-prototypes -mfpmath=387 -mcmodel=large \
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast \
		   -mpreferred-stack-boundary=3 -fno-toplevel-reorder -fno-toplevel-reorder -fno-tree-scev-cprop -Os

CPPFLAGS := -m64 -Wall -Wno-int-conversion -Wstrict-prototypes -mfpmath=387 -mcmodel=large \
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast \
		   -mpreferred-stack-boundary=3 -fno-toplevel-reorder -fno-toplevel-reorder -fno-tree-scev-cprop -Os

LIBS_C_SRC := ctype string tostring debug
MEMORY_C_SRC := kheap pmm paging
INTTERUP_C_SRC := clock/clock exception init_int irq_handler
TASK_C_SRC := task_scheduler task_manager
SYSCALL_C_SRC := syscall syscalls
C_SRC := kernel printk logging kmod_loader \
	$(foreach src, $(LIBS_C_SRC), libs/$(src)) \
	$(foreach src, $(MEMORY_C_SRC), memory/$(src)) \
	$(foreach src, $(INTTERUP_C_SRC), intterup/$(src)) \
	$(foreach src, $(TASK_C_SRC), task/$(src)) \
	$(foreach src, $(SYSCALL_C_SRC), syscall/$(src))

CPP_SRC :=

ASMFLAGS :=
ASM_SRC := entry intterup/int_handlers syscall/dosyscall

C_DEFS := $(ARCHDEF_C)
C_INCLUDE := -I"$(ROOTDIR)/include/std/" -I"$(ROOTDIR)/include/" -I"$(ROOTDIR)/kernel/" -I"$(ROOTDIR)/libs/libfifo"

CPP_DEFS := $(ARCHDEF_C)
CPP_INCLUDE := -I"$(ROOTDIR)/include/std/" -I"$(ROOTDIR)/include/" -I"$(ROOTDIR)/kernel/" -I"$(ROOTDIR)/libs/libfifo"

LD_FLAGS := -static -L"$(BUILDDIR)/bin/" 
LIBRARIES := -lfifo

ifeq ($(MODE), debug)
	LD_FLAGS += -g
	C_DEFS += -D_DEBUG
else
	LD_FLAGS += -s
	CPP_DEFS += -D_DEBUG
endif

ifeq ($(VBE_MODE), ENABLE)
	C_DEFS += -DVBE_ENABLE
endif

OBJECTS := $(foreach obj, $(ASM_SRC), $(OBJECTSDIR)/$(obj).o) \
           $(foreach obj, $(C_SRC), $(OBJECTSDIR)/$(obj).o) \
		   $(foreach obj, $(CPP_SRC), $(OBJECTSDIR)/$(obj).o)

$(OBJECTSDIR)/%.o : %.cpp
	mkdir -p -v $(dir $@)
	$(GPP) -c $(CPPFLAGS) $(CPP_INCLUDE) $(CPP_DEFS) -o $@ $^

$(OBJECTSDIR)/%.o : %.c
	mkdir -p -v $(dir $@)
	$(GCC) -c $(CFLAGS) $(C_INCLUDE) $(C_DEFS) -o $@ $^

$(OBJECTSDIR)/%.o : %.S
	mkdir -p -v $(dir $@)
	$(ASM) -o $@ $^

.PHONY: build
build: $(OBJECTS)
	$(LD) $(LD_FLAGS) -T kernel.ld -o $(KERNEL_OUTPUT) $^ --start-group $(LIBRARIES) --end-group
ifeq ($(MODE), debug)
	$(OBJCOPY) --only-keep-debug $(KERNEL_OUTPUT) $(KERNEL_SYMBOL_OUTPUT)
	$(OBJCOPY) --strip-debug $(KERNEL_OUTPUT)
endif

.PHONY: clean
clean:
	$(RM) $(KERNEL_OUTPUT) $(OBJECTS)

.PHONY: image
image:
	sudo cp $(KERNEL_OUTPUT) $(TAYHUANGOS_MOUNT_DIR)
