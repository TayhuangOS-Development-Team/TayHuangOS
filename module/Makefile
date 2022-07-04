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



OBJECTSDIR := $(OBJECTSDIR)/module/
BINDIR := $(BINDIR)/module/
MODULEDIR := $(ROOTDIR)/module/
export OBJECTSDIR MODULEDIR

#编译
.PHONY: build
build:
	#$(CHANGE_DIR) drivers ; $(MAKE) build
	$(CHANGE_DIR) setup ; $(MAKE) build
	#$(CHANGE_DIR) test ; $(MAKE) build
	#$(CHANGE_DIR) vdd ; $(MAKE) build

#清理
.PHONY: clean
clean:
	#$(CHANGE_DIR) drivers ; $(MAKE) clean
	$(CHANGE_DIR) setup ; $(MAKE) clean
	#$(CHANGE_DIR) test ; $(MAKE) clean
	#$(CHANGE_DIR) vdd ; $(MAKE) clean

.PHONY: image
image:
	#$(CHANGE_DIR) drivers ; $(MAKE) image
	$(CHANGE_DIR) setup ; $(MAKE) image
	#$(CHANGE_DIR) test ; $(MAKE) image
	#$(CHANGE_DIR) vdd ; $(MAKE) image