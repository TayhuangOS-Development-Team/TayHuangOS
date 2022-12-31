MODULENAME := grub

include $(SCRIPTDIR)/compile.mk
include $(SCRIPTDIR)/util.mk

NASM_SRC :=

LIBS_C_SRC := multiboot2
C_SRC := main heap \
	$(call extract, $(LIBS_C_SRC), libs)

CPP_SRC := 

# C
C_FLAGS += -c -m32 -mregparm=1 -Wall -Wno-int-conversion -Wstrict-prototypes -march=i386 -mfpmath=387\
		   -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		   -mno-mmx -mno-sse -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast -mno-red-zone\
		   -mpreferred-stack-boundary=2 -fno-toplevel-reorder -fno-tree-scev-cprop -Os
C_INCLUDE += -I"$(CUR_DIR)" -I"$(ROOTDIR)/include/" -I"$(ROOTDIR)/include/std/" -I"$(TPDIR)/include/"
C_DEFS += $(ARCHDEF_C) -DBITS=32
ifeq ($(VBE_MODE), ENABLE)
	C_DEFS += -DVBE_ENABLE
endif
C_FLAGS += $(call make_c_flags)

# C++
CPP_FLAGS += -c -m32 -mregparm=1 -Wall -fno-rtti -fno-exceptions -march=i386 -mfpmath=387\
		     -fno-strict-aliasing -fomit-frame-pointer -fno-pic -fno-asynchronous-unwind-tables \
 		     -mno-mmx -mno-sse -ffreestanding -fno-stack-protector -Wno-int-to-pointer-cast -mno-red-zone\
		     -mpreferred-stack-boundary=2 -fno-toplevel-reorder -fno-tree-scev-cprop -Os -Wno-pointer-arith
CPP_INCLUDE += -I"$(CUR_DIR)" -I"$(ROOTDIR)/include/" -I"$(ROOTDIR)/include/std/" -I"$(TPDIR)/include/"
CPP_DEFS += $(ARCHDEF_C) -DBITS=32
CPP_FLAGS += $(call make_cpp_flags)

# LD

LD_FLAGS += -s -z max-page-size=0x100
LIBRARIES +=
LD_SCRIPT := grub.ld
LD_FLAGS += $(call make_ld_flags)

# Doxygen
DOXYGEN_CONFIG := DoxygenFile

OUTPUT := $(BINDIR)/grubld.bin

OBJECTS += $(call make_objects, $(NASM_SRC))
OBJECTS += $(call make_objects, $(C_SRC))
OBJECTS += $(call make_objects, $(CPP_SRC))

build: $(OBJECTS)
	$(call link, $(OBJECTS), $(OUTPUT))

clean:
	$(RM) $(OUTPUT) $(OBJECTS)

image:
	$(SUDO) $(COPY) $(OUTPUT) $(TAYHUANGOS_MOUNT_DIR)

doc:
	$(DOXYGEN) $(DOXYGEN_CONFIG)