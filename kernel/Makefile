OBJECTSDIR := $(OBJECTSDIR)/kernel/

KERNEL_OUTPUT := $(BINDIR)/kernel.bin
ASM := as
RM := rm
GCC := gcc
GPP := g++
LD := ld

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
TOOL_C_SRC := tool/tostring
INTTERUP_C_SRC := intterup/init_int intterup/irq_handler intterup/exception
INIT_C_SRC := init/init_clock
PROCESS_C_SRC := process/vmareas process/task_manager
KEYBOARD_C_SRC := keyboard/keyboard keyboard/keymap
SYSCALL_C_SRC := syscall/syscall syscall/system_api
C_SRC := kernel kheap \
		$(STD_C_SRC) \
		$(MEMORY_C_SRC) \
		$(DISPLAY_C_SRC) \
		$(TOOL_C_SRC) \
		$(INTTERUP_C_SRC) \
		$(INIT_C_SRC) \
		$(PROCESS_C_SRC) \
		$(KEYBOARD_C_SRC) \
		$(SYSCALL_C_SRC)

CPP_SRC :=

ASMFLAGS :=
ASM_SRC := entry intterup/int_handlers syscall/dosyscall

C_DEFS := $(ARCHDEF_C)
C_INCLUDE := -I"$(ROOTDIR)/include/std/"  -I"$(ROOTDIR)/include/"

CPP_DEFS := $(ARCHDEF_C)
CPP_INCLUDE := -I"$(ROOTDIR)/include/std/"  -I"$(ROOTDIR)/include/"

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
	$(LD) -s -T kernel.ld -o $(KERNEL_OUTPUT) $^

.PHONY: clean
clean:
	$(RM) $(KERNEL_OUTPUT) $(OBJECTS)

.PHONY: image
image:
	sudo cp $(KERNEL_OUTPUT) /mnt/tayhuangOS


.PHONY: setup_workspace
setup_workspace:
	sudo mkdir -v -p $(OBJECTSDIR)