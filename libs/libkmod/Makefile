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



OBJECTSDIR := $(OBJECTSDIR)/libkmod/

LIBKMOD_OUTPUT := $(BINDIR)/libkmod.a

CFLAGS := -m64 -Wall -Wno-int-conversion -Wstrict-prototypes -mfpmath=387 -mcmodel=large \
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast -mno-red-zone \
		   -mpreferred-stack-boundary=3 -fno-toplevel-reorder -fno-tree-scev-cprop -Os

CPPFLAGS := -m64 -Wall -Wno-int-conversion -Wstrict-prototypes -mfpmath=387 -mcmodel=large \
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast -mno-red-zone \
		   -mpreferred-stack-boundary=3 -fno-toplevel-reorder -fno-tree-scev-cprop -Os

LIBS_C_SRC := ctype string tostring
DEBUG_C_SRC := logging debug
MEMORY_C_SRC := malloc
SYSCALL_C_SRC := rpc
C_SRC := init printf \
         $(foreach src, $(LIBS_C_SRC), libs/$(src)) \
         $(foreach src, $(DEBUG_C_SRC), debug/$(src)) \
         $(foreach src, $(MEMORY_C_SRC), memory/$(src))

CPP_SRC :=

ASMFLAGS :=
ASM_SRC := entry syscall/syscall

C_DEFS := $(ARCHDEF_C)
C_INCLUDE := -I"$(ROOTDIR)/libs/libkmod" -I"$(ROOTDIR)/include" -I"$(ROOTDIR)/include/std"

CPP_DEFS := $(ARCHDEF_C)
CPP_INCLUDE := -I"$(ROOTDIR)/libs/libkmod" -I"$(ROOTDIR)/include" -I"$(ROOTDIR)/include/std"

ifeq ($(MODE), debug)
	C_DEFS += -D_DEBUG
else
	CPP_DEFS += -D_DEBUG
endif

OBJECTS := $(foreach obj, $(ASM_SRC), $(OBJECTSDIR)/$(obj).o) \
           $(foreach obj, $(C_SRC), $(OBJECTSDIR)/$(obj).o) \
		   $(foreach obj, $(CPP_SRC), $(OBJECTSDIR)/$(obj).o)

$(OBJECTSDIR)/%.o : %.cpp
	$(MKDIR) $(dir $@)
	$(GPP) -c $(CPPFLAGS) $(CPP_INCLUDE) $(CPP_DEFS) -o $@ $^

$(OBJECTSDIR)/%.o : %.c
	$(MKDIR) $(dir $@)
	$(GCC) -c $(CFLAGS) $(C_INCLUDE) $(C_DEFS) -o $@ $^

$(OBJECTSDIR)/%.o : %.S
	$(MKDIR) $(dir $@)
	$(GAS) -o $@ $^

.PHONY: build
build: $(OBJECTS)
	$(MKDIR) $(dir $(LIBKMOD_OUTPUT))
	$(AR) rvs $(LIBKMOD_OUTPUT) $^

.PHONY: clean
clean:
	$(RM) $(LIBKMOD_OUTPUT) $(OBJECTS)
