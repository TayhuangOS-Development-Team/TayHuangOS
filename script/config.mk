#
# SPDX-License-Identifier: LGPL-2.1-only
# -------------------------------*-TayhuangOS-*-----------------------------------
# 
#              Copyright (C) 2022, 2022 TayhuangOS Development Team
# 
# --------------------------------------------------------------------------------
# 
# 作者: theflysong 383494
# 
# config.mk
# 
# 配置文件
# 
#

ifndef (HAS_INCLUDE_CONFIG)
HAS_INCLUDE_CONFIG := true

ARCHITECTURE := x86_64
BOOT_TYPE := BIOS		#可选：BIOS UEFI
ARCHDEF_C := -DARCH_$(ARCHITECTURE) #架构宏
FILESYSTEM ?= fat32
MODE := debug
VBE_MODE := DISABLE
KERNEL_PARTITION_OFFSET := 1048576
IMAGE_SECTORS := 262144
PROGRAM_FORMAT := elf
BUILD_VERSION := 1
COMPILER_PREFIX := $(ARCHITECTURE)-$(PROGRAM_FORMAT)

export ARCHITECTURE ARCHDEF_C MODE VBE_MODE

endif