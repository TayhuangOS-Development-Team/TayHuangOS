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
# module.mk
# 
# 工具
# 
#

include $(SCRIPTDIR)/config.mk

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

export GCC GPP ASM GAS RM MKDIR LD DD _MKFS FILESYSTEM MKFS GRUB_INSTALL SUDO MOUNT UMOUNT ECHO CD COPY OBJCOPY FDISK AR

RAW_ICON := $(BINDIR)/tayicon.raw
TAYHUANG_ICON := $(ROOTDIR)/TayhuangOS.png
TOOLS_DIR := $(ROOTDIR)/tools/
PNG_CONV := $(TOOLS_DIR)/png_converter/converter.py
COUNTER := $(TOOLS_DIR)/build_counter/counter.py
COMMENTS_STAT := $(TOOLS_DIR)/comments_stat/stat.py
GET_LOOP := $(TOOLS_DIR)/get_loop_devices/get_loop.sh