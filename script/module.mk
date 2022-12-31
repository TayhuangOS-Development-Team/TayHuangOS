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
include $(SCRIPTDIR)/tools/tool.mk

OBJECTSDIR := $(OBJECTSDIR)/$(MODULENAME)/
BINDIR := $(BINDIR)/$(MODULENAME)/
CURDIR := $(ROOTDIR)/$(MODULENAME)/
export OBJECTSDIR BINDIR CURDIR

define buildmod
	$(foreach mod, $(1), $(CD) $(mod) && $(MAKE) build)
endef

define imagemod
	$(foreach mod, $(1), $(CD) $(mod) && $(MAKE) image)
endef

define cleanmod
	$(foreach mod, $(1), $(CD) $(mod) && $(MAKE) clean)
endef

define docmod
	$(foreach mod, $(1), $(CD) $(mod) && $(MAKE) doc)
endef

.PHONY: build image clean doc