include $(SCRIPTDIR)/util.mk

# 汇编

ASM_SRC :=

# C
C_SRC :=
C_FLAGS := -m32 -mregparm=1 -Wall -Wno-int-conversion -Wstrict-prototypes -march=i386 -mfpmath=387\
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -mno-mmx -mno-sse -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast -mno-red-zone\
		   -mpreferred-stack-boundary=2 -fno-toplevel-reorder -fno-tree-scev-cprop -Os
C_INCLUDE := -I"$(LOADERDIR)/grub"
C_DEFS := $(ARCHDEF_C) -DLOADER32BIT

ifeq ($(VBE_MODE), ENABLE)
	C_DEFS += -DVBE_ENABLE
endif

# C++
MEMORY_CPP_SRC := heap buffer
LIBS_CPP_SRC := multiboot cpp_abi
CPP_SRC := main \
		$(call extract, $(LIBS_CPP_SRC), libs/)\
		$(call extract, $(MEMORY_CPP_SRC), memory/)
CPP_FLAGS := -m32 -mregparm=1 -Wall -fno-rtti -fno-exceptions -march=i386 -mfpmath=387\
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -mno-mmx -mno-sse -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast -mno-red-zone\
		   -mpreferred-stack-boundary=2 -fno-toplevel-reorder -fno-tree-scev-cprop -Os -Wno-pointer-arith
CPP_INCLUDE := -I"$(LOADERDIR)/grub"
CPP_DEFS := $(ARCHDEF_C) -DLOADER32BIT

# LD

LD_FLAGS := -s -z max-page-size=0x100
LIBRARIES :=
LD_SRCIPT := grub.ld

MODULENAME := grub

# Doxygen
DOXYGEN_CONFIG := DoxygenFile

include $(ROOTDIR)/include/std/include.mk
include $(ROOTDIR)/include/tayhuang/include.mk

include $(SCRIPTDIR)/compile.mk

OUTPUT := $(BINDIR)/grubld.bin

build: $(OBJECTS)
	$(call link, $(OUTPUT), $(OBJECTS))

clean:
	$(RM) $(OUTPUT) $(OBJECTS)

image:
	$(SUDO) $(COPY) $(OUTPUT) $(TAYHUANGOS_MOUNT_DIR)

doc:
	$(DOXYGEN) $(DOXYGEN_CONFIG)