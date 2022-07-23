#
# SPDX-License-Identifier: LGPL-2.1-only
# -------------------------------*-TayhuangOS-*-----------------------------------
# 
#    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
# 
# --------------------------------------------------------------------------------
# 
# 作者: theflysong 383494
# 
# Makefile
# 
# Makefile
# 
#



# 配置区

#include ./config.mk

#定义区

ARCHITECTURE ?= x86_64
BOOT_TYPE := BIOS		#可选：BIOS UEFI
ARCHDEF_C := -DARCH_$(ARCHITECTURE) #架构宏
FILESYSTEM ?= fat32
MODE ?= debug
VBE_MODE ?= DISABLE
KERNEL_PARTITION_OFFSET ?= 1048576
IMAGE_SECTORS ?= 262144
PROGRAM_FORMAT ?= elf
COMPILER_PREFIX := $(ARCHITECTURE)-$(PROGRAM_FORMAT)
# 这里本来有LOOPA和LOOPB，但依赖于get_loop.sh，故放在工具区
CODE_VERSION  := alpha
MAJOR_VERSION := 2
MINOR_VERSION := 25
PATCH_VERSION := 8
BUILD_VERSION := 2

VERSION := $(CODE_VERSION)-$(MAJOR_VERSION).$(MINOR_VERSION).$(PATCH_VERSION):build $(BUILD_VERSION)

export ARCHITECTURE ARCHDEF_C MODE VBE_MODE CODE_VERSION MAJOR_VERSION MINOR_VERSION PATCH_VERSION BUILD_VERSION VERSION

#目录区

ROOTDIR := $(shell pwd)
BUILDDIR := $(ROOTDIR)/build/$(MODE)/
BINDIR := $(BUILDDIR)/bin/
OBJECTSDIR := $(BUILDDIR)/objects/
TAYHUANGOS_MOUNT_DIR := /mnt/tayhuangOS
TAYHUANGOS_IMG := tayhuangOS.img

#工具区

MKDIR := mkdir -p -v
GCC := $(COMPILER_PREFIX)-gcc
GPP := $(COMPILER_PREFIX)-g++
ASM := nasm
GAS := $(COMPILER_PREFIX)-as
RM := rm
LD := $(COMPILER_PREFIX)-ld
DD := dd
_MKFS := mkfs
GRUB_INSTALL := grub-install
GRUB_MODULES := normal part_msdos ext2 fat multiboot all_video
SUDO := sudo
MOUNT := mount
UMOUNT := umount
ECHO := echo
CD := cd
COPY := cp
OBJCOPY := $(COMPILER_PREFIX)-objcopy
FDISK := fdisk
LOOP_SETUP := losetup
CHMOD := chmod
AR := $(COMPILER_PREFIX)-ar

ifeq ($(ARCHITECTURE), x86_64)
	QEMU := qemu-system-x86_64
endif

QEMU_ARGS ?=
QEMU_ARGS += -m size=256m,maxmem=256m \
			 --enable-kvm \
             -hda tayhuangOS.img \
			 -serial stdio \
			 -rtc base=localtime \
			 -name "TayhuangOS"

QEMU_DEBUG_ARGS := $(QEMU_ARGS) -s -S

ifeq ($(FILESYSTEM), fat32)
	MKFS := $(_MKFS).msdos -F 32 -n "TayhuangOS" -v -f 2
endif

export ROOTDIR BUILDDIR BINDIR OBJECTSDIR TAYHUANGOS_MOUNT_DIR TAYHUANGBOOT_MOUNT_DIR
export GCC GPP ASM GAS RM MKDIR LD DD _MKFS FILESYSTEM MKFS GRUB_INSTALL SUDO MOUNT UMOUNT ECHO CD COPY OBJCOPY FDISK AR

RAW_ICON := $(BINDIR)/tayicon.raw
TAYHUANG_ICON := $(ROOTDIR)/TayhuangOS.png
TOOLS_DIR := $(ROOTDIR)/tools/
PNG_CONV := $(TOOLS_DIR)/png_converter/converter.py
COUNTER := $(TOOLS_DIR)/build_counter/counter.py
COMMENTS_STAT := $(TOOLS_DIR)/comments_stat/stat.py
GET_LOOP := $(TOOLS_DIR)/get_loop_devices/get_loop.sh

# 本来应该放在定义区，但依赖于get_loop.sh
LOOPA ?= $(shell $(GET_LOOP) 0)
LOOPB ?= $(shell $(GET_LOOP) $(LOOPA))

# 任务区
# 下划线结尾的任务不应直接调用

#编译并写入映像
.PHONY: all
all: build image
	$(ECHO) "done"

#重编译并写入映像
.PHONY: rebuild
rebuild: clean build image
	$(ECHO) "done"

#设置环境并构建
.PHONY: setup_and_build
setup_and_build: setup_workspace all
	$(ECHO) "done"

#设置环境
.PHONY: setup_workspace
setup_workspace:
	$(ECHO) "TayhuangOS Version: $(VERSION)"
	if [ ! -d "$(TAYHUANGOS_MOUNT_DIR)" ];then \
		$(SUDO) $(MKDIR) $(TAYHUANGOS_MOUNT_DIR); \
	else \
		$(ECHO) "mount directory already created"; \
	fi;
	$(DD) if=/dev/zero of=$(TAYHUANGOS_IMG) bs=512 count=$(IMAGE_SECTORS)

	$(MKDIR) $(BUILDDIR)
	$(MKDIR) $(OBJECTSDIR)
	$(MKDIR) $(BINDIR)

	$(FDISK) $(TAYHUANGOS_IMG)
	-$(MAKE) setup_workspace_losetup_

	-$(SUDO) $(LOOP_SETUP) -d $(LOOPB)	#losetup LOOPA失败不会影响LOOPB
	$(SUDO) $(LOOP_SETUP) -d $(LOOPA)

	$(SUDO) $(CHMOD) +x $(PNG_CONV)
	$(SUDO) $(CHMOD) +x $(COUNTER)
	$(SUDO) $(CHMOD) +x $(COMMENTS_STAT)

.PHONY: setup_workspace_losetup_
setup_workspace_losetup_:
	$(SUDO) $(LOOP_SETUP) $(LOOPA) $(TAYHUANGOS_IMG)
	$(SUDO) $(LOOP_SETUP) $(LOOPB) $(TAYHUANGOS_IMG) -o $(KERNEL_PARTITION_OFFSET)
	
	$(SUDO) $(MKFS) $(LOOPB)
	$(SUDO) $(MOUNT) $(LOOPB) $(TAYHUANGOS_MOUNT_DIR)

	#TODO:添加UEFI支持
	-$(SUDO) $(GRUB_INSTALL) --target=i386-pc --root-directory=$(TAYHUANGOS_MOUNT_DIR) --no-floppy --modules="$(GRUB_MODULES)" $(LOOPA)

	$(SUDO) $(UMOUNT) $(TAYHUANGOS_MOUNT_DIR)



.PHONY: do_count
do_count:
	$(COUNTER)
	$(ECHO) "TayhuangOS Version: $(VERSION)"

.PHONY: show_version
show_version:
	$(ECHO) "TayhuangOS Version: $(VERSION)"

.PHONY: stat_code_density
stat_code_density:
	$(COMMENTS_STAT)

#编译
.PHONY: build
build: do_count
	$(ECHO) "TayhuangOS Version: $(VERSION)"
	$(CD) libs && $(MAKE) build
	$(CD) loader && $(MAKE) build
	$(CD) kernel && $(MAKE) build
	$(CD) module && $(MAKE) build
ifeq ($(VBE_MODE), ENABLE)
	$(MAKE) $(RAW_ICON)
endif

$(RAW_ICON): $(TAYHUANG_ICON)
	$(PNG_CONV) $(TAYHUANG_ICON) $(RAW_ICON)

#清理
.PHONY: clean
clean:
	$(ECHO) "TayhuangOS Version: $(VERSION)"
	-$(CD) libs && $(MAKE) clean
	-$(CD) loader && $(MAKE) clean
	-$(CD) kernel && $(MAKE) clean
	-$(CD) module && $(MAKE) clean

#写入映像
.PHONY: image
image:
	$(ECHO) "TayhuangOS Version: $(VERSION)"
	-$(MAKE) image_losetup_
	-$(SUDO) $(UMOUNT) $(TAYHUANGOS_MOUNT_DIR)
	-$(SUDO) $(LOOP_SETUP) -d $(LOOPB)

.PHONY: image_losetup_
image_losetup_:
	$(SUDO) $(LOOP_SETUP) $(LOOPB) $(TAYHUANGOS_IMG) -o 1048576
	$(SUDO) $(MOUNT) $(LOOPB) $(TAYHUANGOS_MOUNT_DIR)
	$(SUDO) $(COPY) ./configs/grub.cfg $(TAYHUANGOS_MOUNT_DIR)/boot/grub
ifeq ($(VBE_MODE), ENABLE)
	$(SUDO) $(COPY) $(RAW_ICON) $(TAYHUANGOS_MOUNT_DIR)/
endif

	$(CD) loader && $(MAKE) image
	$(CD) kernel && $(MAKE) image
	$(CD) module && $(MAKE) image

.PHONY: run
run:
	$(ECHO) "TayhuangOS Version: $(VERSION)"
	$(QEMU) $(QEMU_ARGS)

.PHONY: debug
debug:
	$(ECHO) "TayhuangOS Version: $(VERSION)"
	$(QEMU) $(QEMU_DEBUG_ARGS)

.PHONY: build_and_run
build_and_run: all run
	$(ECHO) "done"

.PHONY: build_and_debug
build_and_debug: all debug
	$(ECHO) "done"
