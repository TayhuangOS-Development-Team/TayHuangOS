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
# version.mk
# 
# 版本
# 
#

CODE_VERSION  := alpha
MAJOR_VERSION := 1
MINOR_VERSION := 0
PATCH_VERSION := 0

VERSION := $(CODE_VERSION)-$(MAJOR_VERSION).$(MINOR_VERSION).$(PATCH_VERSION):build $(BUILD_VERSION)

export CODE_VERSION MAJOR_VERSION MINOR_VERSION PATCH_VERSION BUILD_VERSION VERSION