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
	$(CD) driver && $(MAKE) build
	$(CD) setup && $(MAKE) build

#清理
.PHONY: clean
clean:
	$(CD) driver && $(MAKE) clean
	$(CD) setup && $(MAKE) clean

.PHONY: image
image:
	$(CD) driver && $(MAKE) image
	$(CD) setup && $(MAKE) image