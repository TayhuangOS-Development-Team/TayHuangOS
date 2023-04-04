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
# util.mk
# 
# 工具
# 
#

ifndef (HAS_INCLUDE_UTIL)
HAS_INCLUDE_UTIL := true

define extract
	$(foreach src, $(1), $(2)/$(src))
endef

endif