OBJECTSDIR := $(OBJECTSDIR)/libc/

LIBC_OUTPUT := $(BINDIR)/libc.a

CFLAGS := -m64 -Wall -Wno-int-conversion -Wstrict-prototypes -mfpmath=387 -mcmodel=large \
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast \
		   -mpreferred-stack-boundary=3 -fno-toplevel-reorder -fno-toplevel-reorder -fno-tree-scev-cprop -Os

CPPFLAGS := -m64 -Wall -Wno-int-conversion -Wstrict-prototypes -mfpmath=387 -mcmodel=large \
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast \
		   -mpreferred-stack-boundary=3 -fno-toplevel-reorder -fno-toplevel-reorder -fno-tree-scev-cprop -Os

MISC_C_SRC := misc/timer
C_SRC := $(MISC_C_SRC)

CPP_SRC :=

ASMFLAGS :=
ASM_SRC := ipc/ipc_func

C_DEFS := $(ARCHDEF_C)
C_INCLUDE := -I"$(ROOTDIR)/libs/libc"

CPP_DEFS := $(ARCHDEF_C)
CPP_INCLUDE := -I"$(ROOTDIR)/libs/libc"

OBJECTS := $(foreach obj, $(ASM_SRC), $(OBJECTSDIR)/$(obj).o) \
           $(foreach obj, $(C_SRC), $(OBJECTSDIR)/$(obj).o) \
		   $(foreach obj, $(CPP_SRC), $(OBJECTSDIR)/$(obj).o)

$(OBJECTSDIR)/%.o : %.cpp
	$(MKDIR) -p -v $(dir $@)
	$(GPP) -c $(CPPFLAGS) $(CPP_INCLUDE) $(CPP_DEFS) -o $@ $^

$(OBJECTSDIR)/%.o : %.c
	$(MKDIR) -p -v $(dir $@)
	$(GCC) -c $(CFLAGS) $(C_INCLUDE) $(C_DEFS) -o $@ $^

$(OBJECTSDIR)/%.o : %.S
	$(MKDIR) -p -v $(dir $@)
	$(GAS) -o $@ $^

.PHONY: build
build: $(OBJECTS)
	$(AR) rvs $(LIBC_OUTPUT) $^

.PHONY: clean
clean:
	$(RM) $(LIBC_OUTPUT) $(OBJECTS)
