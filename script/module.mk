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
# 模块
# 
#

# 需要定义 MODULENAME

OBJECTSDIR := $(OBJECTSDIR)/$(MODULENAME)/
BINDIR := $(BINDIR)/$(MODULENAME)/
CURDIR := $(ROOTDIR)/$(MODULENAME)/
export OBJECTSDIR BINDIR CURDIR