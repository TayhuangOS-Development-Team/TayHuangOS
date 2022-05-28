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

STD_C_SRC := std/ctype std/string std/math
MEMORY_C_SRC := memory/paging memory/pmm
DISPLAY_C_SRC := display/printk display/video
TOOL_C_SRC := tool/tostring tool/debug
INTTERUP_C_SRC := intterup/init_int intterup/irq_handler intterup/exception
INIT_C_SRC := init/init_clock
PROCESS_C_SRC := process/vmareas process/task_manager
SYSCALL_C_SRC := syscall/syscall
DRIVER_C_SRC := driver/video/video driver/clock/clock driver/keyboard/keyboard driver/keyboard/keymap
TEST_C_SRC := test/test_proccess
DEBUG_C_SRC := debug/logging
C_SRC := kernel kheap \
		$(STD_C_SRC) \
		$(MEMORY_C_SRC) \
		$(DISPLAY_C_SRC) \
		$(TOOL_C_SRC) \
		$(INTTERUP_C_SRC) \
		$(INIT_C_SRC) \
		$(PROCESS_C_SRC) \
		$(KEYBOARD_C_SRC) \
		$(SYSCALL_C_SRC) \
		$(TEST_C_SRC) \
		$(DRIVER_C_SRC) \
		$(DEBUG_C_SRC)

CPP_SRC :=

ASMFLAGS :=
ASM_SRC := entry intterup/int_handlers syscall/dosyscall

C_DEFS := $(ARCHDEF_C)
C_INCLUDE := -I"$(ROOTDIR)/include/std/"  -I"$(ROOTDIR)/include/" -I"$(ROOTDIR)/kernel/" -I"$(ROOTDIR)/kernel/driver/"

CPP_DEFS := $(ARCHDEF_C)
CPP_INCLUDE := -I"$(ROOTDIR)/include/std/"  -I"$(ROOTDIR)/include/" -I"$(ROOTDIR)/kernel/" -I"$(ROOTDIR)/kernel/driver/"

LD_FLAGS :=

ifeq ($(MODE), debug)
	LD_FLAGS := -g
	C_DEFS += -D_DEBUG
else
	LD_FLAGS := -s
	CPP_DEFS += -D_DEBUG
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
	$(LD) $(LD_FLAGS) -T kernel.ld -o $(KERNEL_OUTPUT) $^
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
