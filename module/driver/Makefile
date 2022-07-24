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



OBJECTSDIR := $(OBJECTSDIR)/driver/
BINDIR := $(BINDIR)/driver/
DRIVERDIR := $(MODULEDIR)/driver/
export OBJECTSDIR BINDIR DRIVERDIR

#编译
.PHONY: build
build:
	$(CD) video && $(MAKE) build
	$(CD) keyboard && $(MAKE) build

#清理
.PHONY: clean
clean:
	$(CD) video && $(MAKE) clean
	$(CD) keyboard && $(MAKE) clean

.PHONY: image
image:
	$(CD) video && $(MAKE) image
	$(CD) keyboard && $(MAKE) image