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

ifndef (HAS_INCLUDE_COMPILE)
HAS_INCLUDE_COMPILE := true

include $(SCRIPTDIR)/module.mk
include $(SCRIPTDIR)/tools/c_tools.mk
include $(SCRIPTDIR)/tools/cpp_tools.mk
include $(SCRIPTDIR)/tools/gas_tools.mk
include $(SCRIPTDIR)/tools/ld_tools.mk
include $(SCRIPTDIR)/tools/ar_tools.mk
include $(SCRIPTDIR)/tools/nasm_tools.mk
include $(SCRIPTDIR)/tools/tool.mk

define make_objects
	$(foreach obj, $(1), $(OBJECTSDIR)/$(obj).o)
endef

endif