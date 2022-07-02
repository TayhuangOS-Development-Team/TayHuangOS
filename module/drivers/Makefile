#
# SPDX-License-Identifier: GPL-3.0-only
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



OBJECTSDIR := $(OBJECTSDIR)/drivers/
BINDIR := $(BINDIR)/drivers/
DRIVERSDIR := $(MODULEDIR)/drivers/
export OBJECTSDIR DRIVERSDIR

#编译
.PHONY: build
build:
	$(CHANGE_DIR) ./video/ ; $(MAKE) build
	$(CHANGE_DIR) ./keyboard/ ; $(MAKE) build

#清理
.PHONY: clean
clean:
	$(CHANGE_DIR) ./video/ ; $(MAKE) clean
	$(CHANGE_DIR) ./keyboard/ ; $(MAKE) clean

.PHONY: image
image:
	$(CHANGE_DIR) ./video/ ; $(MAKE) image
	$(CHANGE_DIR) ./keyboard/ ; $(MAKE) image