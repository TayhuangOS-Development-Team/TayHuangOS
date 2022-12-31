#
# SPDX-License-Identifier: LGPL-2.1-only
# -------------------------------*-TayhuangOS-*-----------------------------------
# 
#              Copyright (C) 2022, 2022 TayhuangOS Development Team
# 
# --------------------------------------------------------------------------------
# 
# 作者: theflysong
# 
# gas_tools.mk
# 
# 模块
# 
#

include $(SCRIPTDIR)/tools/tool.mk

GAS_COMPILER := $(COMPILER_PREFIX)-as
GAS_FLAGS :=

define gas_compile
	$(GAS_COMPILER) $(GAS_FLAGS) -o $(2) $(1)
endef

$(OBJECTSDIR)/%.o : %.S
	$(MKDIR) $(dir $@)
	$(call gas_compile, $^, $@)