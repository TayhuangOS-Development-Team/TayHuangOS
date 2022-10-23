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
# compile.mk
# 
# 编译
# 
#

# 需要定义 ASM_SRC 
# 需要定义 MODULENAME 
# 需要定义 C_SRC C_FLAGS C_INCLUDE C_DEFS
# 需要定义 CPP_SRC CPP_FLAGS CPP_INCLUDE CPP_DEFS
# 需要定义 LD_FLAGS LIBRARIES LD_SRCIPT 

include $(SCRIPTDIR)/module.mk
include $(SCRIPTDIR)/tool.mk

OBJECTS := $(foreach obj, $(ASM_SRC), $(OBJECTSDIR)/$(obj).o) \
           $(foreach obj, $(C_SRC), $(OBJECTSDIR)/$(obj).o) \
		   $(foreach obj, $(CPP_SRC), $(OBJECTSDIR)/$(obj).o)

$(OBJECTSDIR)/%.o : %.cpp
	$(MKDIR) $(dir $@)
	$(GPP) -c $(CPP_FLAGS) $(CPP_INCLUDE) $(CPP_DEFS) -o $@ $^

$(OBJECTSDIR)/%.o : %.c
	$(MKDIR) $(dir $@)
	$(GCC) -c $(C_FLAGS) $(C_INCLUDE) $(C_DEFS) -o $@ $^

$(OBJECTSDIR)/%.o : %.S
	$(MKDIR) $(dir $@)
	$(GAS) -o $@ $^

define link
	$(MKDIR) $(dir $(1))
	$(LD) $(LD_FLAGS) -T $(LD_SRCIPT) -o $(1) $(2) --start-group $(LIBRARIES) --end-group
endef