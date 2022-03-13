KERNEL_OUTPUT := kernel.bin
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
C_SRC := kernel kheap \
		$(STD_C_SRC) \
		$(MEMORY_C_SRC) \
		$(DISPLAY_C_SRC) \
		$(TOOL_C_SRC) \
		$(INTTERUP_C_SRC) \
		$(INIT_C_SRC) \
		$(PROCESS_C_SRC)

CPP_SRC :=

ASMFLAGS :=
ASM_SRC := entry intterup/int_handlers

C_DEFS := $(ARCHDEF_C)
C_INCLUDE := -I"../include/std/"  -I"../include/"

CPP_DEFS := $(ARCHDEF_C)
CPP_INCLUDE := -I"../include/std/"  -I"../include/"


.PHONY: build
build:
	for asm_file in $(ASM_SRC); do \
		$(ASM) $$asm_file.S -o $$asm_file.o; \
	done
	for c_file in $(C_SRC); do \
	    $(GCC) $(CFLAGS) $(C_INCLUDE) $(C_DEFS) $$c_file.c -c -o $$c_file.o;\
	done
	for cpp_file in $(CPP_SRC); do \
	    $(GPP) $(CPPFLAGS) $(CPP_INCLUDE) $(CPP_DEFS) $$cpp_file.cpp -c -o $$cpp_file.o;\
	done
	$(LD) -s -T kernel.ld $(foreach asm_obj, $(ASM_SRC), $(asm_obj).o) $(foreach c_obj, $(C_SRC), $(c_obj).o) $(foreach cpp_obj, $(CPP_SRC), $(cpp_obj).o) -o $(KERNEL_OUTPUT)

.PHONY: clean
clean:
	for asm_file in $(ASM_SRC); do \
		$(RM) $$asm_file.o; \
	done
	for c_file in $(C_SRC); do \
	    $(RM) $$c_file.o; \
	done
	for cpp_file in $(CPP_SRC); do \
	    $(RM) $$cpp_file.o; \
	done
	$(RM) $(KERNEL_OUTPUT)

.PHONY: image
image:
	sudo cp $(KERNEL_OUTPUT) /mnt/tayhuangOS