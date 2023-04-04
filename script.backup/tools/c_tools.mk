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
# c_tools.mk
# 
# 模块
# 
#

ifndef (HAS_INCLUDE_TOOLS_C)
HAS_INCLUDE_TOOLS_C := true

include $(SCRIPTDIR)/tools/tool.mk

C_COMPILER := $(COMPILER_PREFIX)-gcc
C_WARNINGS :=
C_ERRORS :=
C_FUNCTIONS :=
C_DEFS :=
C_FLAGS :=
C_INCLUDE :=

define make_c_flags
	$(C_WARNINGS) $(C_ERRORS) $(C_FUNCTIONS) $(C_DEFS) $(C_INCLUDE)
endef

define c_compile
	$(C_COMPILER) $(C_FLAGS) -o $(2) $(1)
endef

$(OBJECTSDIR)/%.o : %.c
	$(MKDIR) $(dir $@)
	$(call c_compile, $^, $@)

endif