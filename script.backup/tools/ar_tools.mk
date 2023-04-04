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
# ar_tools.mk
# 
# 模块
# 
#

ifndef (HAS_INCLUDE_TOOLS_AR)
HAS_INCLUDE_TOOLS_AR := true

include $(SCRIPTDIR)/tools/tool.mk

AR_COMPILER := $(COMPILER_PREFIX)-ar
AR_FLAGS := rcs

define ar_link
	$(MKDIR) $(dir $(2))
	$(AR_COMPILER) $(AR_FLAGS) $(2) $(1)
endef

endif