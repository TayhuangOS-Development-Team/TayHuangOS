include $(SCRIPTDIR)/util.mk

# 汇编

ASM_SRC := handler flush

# C
FS_C_SRC := disk vfs
LIBS_C_SRC := ctype string tostring
INIT_C_SRC := gdt intterup
C_SRC := main printf logging heap \
		 $(call extract, $(LIBS_C_SRC), libs) \
		 $(call extract, $(INIT_C_SRC), init) \
		 $(call extract, $(FS_C_SRC), fs)
C_FLAGS := -m32 -mregparm=1 -Wall -Wno-int-conversion -Wstrict-prototypes -march=i386 -mfpmath=387\
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -mno-mmx -mno-sse -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast -mno-red-zone\
		   -mpreferred-stack-boundary=2 -fno-toplevel-reorder -fno-tree-scev-cprop -Os
C_INCLUDE := -I"$(ROOTDIR)/include/std/" -I"$(ROOTDIR)/include" -I"$(LOADERDIR)/grub"
C_DEFS := $(ARCHDEF_C) -DLOADER32BIT

ifeq ($(VBE_MODE), ENABLE)
	C_DEFS += -DVBE_ENABLE
endif

# C++

CPP_SRC := 
CPP_FLAGS :=
CPP_INCLUDE :=
CPP_DEFS :=

# LD

LD_FLAGS := -s -z max-page-size=0x100
LIBRARIES :=
LD_SRCIPT := grub.ld

MODULENAME := grub

# Doxygen
DOXYGEN_CONFIG := DoxygenFile

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