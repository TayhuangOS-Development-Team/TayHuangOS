#
# SPDX-License-Identifier: LGPL-2.1-only
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



OBJECTSDIR := $(OBJECTSDIR)/testbench1/

VIDEO_OUTPUT := $(BINDIR)/tb1.mod
VIDEO_SYMBOL_OUTPUT := $(BINDIR)/tb1.sym

CFLAGS := -m64 -Wall -Wno-int-conversion -Wstrict-prototypes -mfpmath=387 -mcmodel=large \
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast -mno-red-zone \
		   -mpreferred-stack-boundary=3 -fno-toplevel-reorder -fno-tree-scev-cprop -Os

CPPFLAGS := -m64 -Wall -Wno-int-conversion -Wstrict-prototypes -mfpmath=387 -mcmodel=large \
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast -mno-red-zone \
		   -mpreferred-stack-boundary=3 -fno-toplevel-reorder -fno-tree-scev-cprop -Os 

C_SRC := main

CPP_SRC :=

ASMFLAGS :=
ASM_SRC :=

C_DEFS := $(ARCHDEF_C)
C_INCLUDE := -I"$(TESTBENCHDIR)/testbench1" -I"$(ROOTDIR)/libs/libkmod" -I"$(ROOTDIR)/libs/libfifo" -I"$(ROOTDIR)/include" -I"$(ROOTDIR)/include/std"

ifeq ($(MODE), debug)
	C_DEFS += -D_DEBUG
else
	CPP_DEFS += -D_DEBUG
endif

CPP_DEFS := $(ARCHDEF_C)
CPP_INCLUDE := -I"$(TESTBENCHDIR)/testbench1" -I"$(ROOTDIR)/libs/libkmod" -I"$(ROOTDIR)/libs/libfifo" -I"$(ROOTDIR)/include" -I"$(ROOTDIR)/include/std"

LD_FLAGS := -static -L"$(BUILDDIR)/bin/" -z max-page-size=0x1000
LIBRARIES := -lkmod -lfifo

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
	$(MKDIR) $(dir $(VIDEO_OUTPUT))
	$(LD) $(LD_FLAGS) -T testbench1.ld -o $(VIDEO_OUTPUT) $^ --start-group $(LIBRARIES) --end-group
ifeq ($(MODE), debug)
	$(OBJCOPY) --only-keep-debug $(VIDEO_OUTPUT) $(VIDEO_SYMBOL_OUTPUT)
	$(OBJCOPY) --strip-debug $(VIDEO_OUTPUT)
endif

.PHONY: clean
clean:
	$(RM) $(VIDEO_OUTPUT) $(OBJECTS)

.PHONY: image
image:
	$(SUDO) $(COPY) $(VIDEO_OUTPUT) $(TAYHUANGOS_MOUNT_DIR)
