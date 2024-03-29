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



OBJECTSDIR := $(OBJECTSDIR)/setup/

SETUP_OUTPUT := $(BINDIR)/setup.mod
SETUP_SYMBOL_OUTPUT := $(BINDIR)/setup.sym

CFLAGS := -m64 -Wall -Wno-int-conversion -Wstrict-prototypes -mfpmath=387 -mcmodel=large \
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast -mno-red-zone \
		   -mpreferred-stack-boundary=3 -fno-toplevel-reorder -fno-tree-scev-cprop -Os

CPPFLAGS := -m64 -Wall -Wno-int-conversion -Wstrict-prototypes -mfpmath=387 -mcmodel=large \
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast -mno-red-zone \
		   -mpreferred-stack-boundary=3 -fno-toplevel-reorder -fno-tree-scev-cprop -Os 

_LIBKMOD_MEMORY_SRC := malloc
_LIBKMOD_LIBS_SRC := ctype string tostring
_LIBKMOD_DEBUG_SRC := debug logging
_LIBKMOD_C_SRC := init printf \
	$(foreach src, $(_LIBKMOD_MEMORY_SRC), memory/$(src)) \
	$(foreach src, $(_LIBKMOD_LIBS_SRC), libs/$(src)) \
	$(foreach src, $(_LIBKMOD_DEBUG_SRC), debug/$(src))
FS_C_SRC := common fat32
C_SRC := main disk \
	$(foreach src, $(FS_C_SRC), fs/$(src)) \
	$(foreach src, $(_LIBKMOD_C_SRC), _libkmod/$(src))

CPP_SRC :=

ASMFLAGS :=
ASM_SRC := _libkmod/entry _libkmod/syscall/syscall

C_DEFS := $(ARCHDEF_C)
C_INCLUDE := -I"$(MODULEDIR)/setup" -I"$(MODULEDIR)/setup/_libkmod" -I"$(ROOTDIR)/include" -I"$(ROOTDIR)/include/std"

CPP_DEFS := $(ARCHDEF_C)
CPP_INCLUDE := -I"$(MODULEDIR)/setup" -I"$(MODULEDIR)/setup/_libkmod" -I"$(ROOTDIR)/include" -I"$(ROOTDIR)/include/std"

LD_FLAGS := -static -L"$(BUILDDIR)/bin/" -z max-page-size=0x1000
LIBRARIES :=

OBJECTS := $(foreach obj, $(ASM_SRC), $(OBJECTSDIR)/$(obj).o) \
           $(foreach obj, $(C_SRC), $(OBJECTSDIR)/$(obj).o) \
		   $(foreach obj, $(CPP_SRC), $(OBJECTSDIR)/$(obj).o)

ifeq ($(MODE), debug)
	C_DEFS += -D_DEBUG
else
	CPP_DEFS += -D_DEBUG
endif

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
	$(MKDIR) $(dir $(SETUP_OUTPUT))
	$(LD) $(LD_FLAGS) -T setup.ld -o $(SETUP_OUTPUT) $^ --start-group $(LIBRARIES) --end-group
ifeq ($(MODE), debug)
	$(OBJCOPY) --only-keep-debug $(SETUP_OUTPUT) $(SETUP_SYMBOL_OUTPUT)
	$(OBJCOPY) --strip-debug $(SETUP_OUTPUT)
endif

.PHONY: clean
clean:
	$(RM) $(SETUP_OUTPUT) $(OBJECTS)

.PHONY: image
image:
	$(SUDO) $(COPY) $(SETUP_OUTPUT) $(TAYHUANGOS_MOUNT_DIR)
