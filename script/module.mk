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
# module.mk
# 
# 模块
# 
#

# 需要定义 MODULENAME

include $(SCRIPTDIR)/directory.mk
include $(SCRIPTDIR)/tool.mk

OBJECTSDIR := $(OBJECTSDIR)/$(MODULENAME)/
BINDIR := $(BINDIR)/$(MODULENAME)/
CURDIR := $(ROOTDIR)/$(MODULENAME)/
export OBJECTSDIR BINDIR CURDIR


define buildmod
	$(CD) $(1) ; $(MAKE) build
endef

define imagemod
	$(CD) $(1) ; $(MAKE) image
endef

define cleanmod
	$(CD) $(1) ; $(MAKE) clean
endef

.PHONY: build image clean