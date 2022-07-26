#
# SPDX-License-Identifier: LGPL-2.1-only
# -------------------------------*-TayhuangOS-*-----------------------------------
# 
#    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
# 
# --------------------------------------------------------------------------------
# 
# 作者: theflysong
# 
# Makefile
# 
# Makefile
# 
#




#编译
.PHONY: build
build:
	$(CD) ioman && $(MAKE) build
	$(CD) driver && $(MAKE) build

#清理
.PHONY: clean
clean:
	$(CD) ioman && $(MAKE) clean
	$(CD) driver && $(MAKE) clean

.PHONY: image
image:
	$(CD) ioman && $(MAKE) image
	$(CD) driver && $(MAKE) image