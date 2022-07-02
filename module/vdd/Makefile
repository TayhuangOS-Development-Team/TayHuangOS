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



OBJECTSDIR := $(OBJECTSDIR)/vdd/
BINDIR := $(BINDIR)/vdd/
VDDDIR := $(MODULEDIR)/vdd/
export OBJECTSDIR VDDDIR

#编译
.PHONY: build
build:
	$(CHANGE_DIR) tty ; $(MAKE) build

#清理
.PHONY: clean
clean:
	$(CHANGE_DIR) tty ; $(MAKE) clean

.PHONY: image
image:
	$(CHANGE_DIR) tty ; $(MAKE) image