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
# nasm_tools.mk
# 
# 模块
# 
#

include $(SCRIPTDIR)/tools/tool.mk

NASM_COMPILER := nasm
NASM_FLAGS :=

define make_nasm_flags
	$(NASM_WARNINGS) $(NASM_ERRORS) $(NASM_FUNCTIONS) $(NASM_DEFS) $(NASM_INCLUDE)
endef

define nasm_compile
	$(NASM_COMPILER) $(NASM_FLAGS) -o $(2) $(1)
endef

$(OBJECTSDIR)/%.o : %.asm
	$(MKDIR) $(dir $@)
	$(call nasm_compile, $^, $@)