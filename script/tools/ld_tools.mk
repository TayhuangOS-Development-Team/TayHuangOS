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
# ld_tools.mk
# 
# 模块
# 
#

ifndef (HAS_INCLUDE_TOOLS_LD)
HAS_INCLUDE_TOOLS_LD := true

include $(SCRIPTDIR)/tools/tool.mk

LD_COMPILER := $(COMPILER_PREFIX)-ld
LIBRARIES :=
LD_SCRIPT :=
LD_FLAGS :=

define make_ld_flags
	-T $(LD_SCRIPT)
endef

define link
	$(MKDIR) $(dir $(2))
	$(LD_COMPILER) $(LD_FLAGS) -o $(2) $(1) --start-group $(LIBRARIES) --end-group
endef

endif