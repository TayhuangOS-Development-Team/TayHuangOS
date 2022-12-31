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
# cpp_tools.mk
# 
# 模块
# 
#

include $(SCRIPTDIR)/tools/tool.mk

CPP_COMPILER := $(COMPILER_PREFIX)-g++
CPP_WARNINGS :=
CPP_ERRORS :=
CPP_FUNCTIONS :=
CPP_DEFS :=
CPP_FLAGS :=
CPP_INCLUDE :=

define make_cpp_flags
	$(CPP_WARNINGS) $(CPP_ERRORS) $(CPP_FUNCTIONS) $(CPP_DEFS) $(CPP_INCLUDE)
endef

define cpp_compile
	$(CPP_COMPILER) $(CPP_FLAGS) -o $(2) $(1)
endef

$(OBJECTSDIR)/%.o : %.cpp
	$(MKDIR) $(dir $@)
	$(call cpp_compile, $^, $@)