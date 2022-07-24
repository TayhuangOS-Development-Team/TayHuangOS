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



OBJECTSDIR := $(OBJECTSDIR)/loader/
BINDIR := $(BINDIR)/loader/
LOADERDIR := $(ROOTDIR)/loader/
export OBJECTSDIR LOADERDIR

#编译
.PHONY: build
build:
	$(CD) grub_loader && $(MAKE) build

#清理
.PHONY: clean
clean:
	$(CD) grub_loader && $(MAKE) clean

.PHONY: image
image:
	$(CD) grub_loader && $(MAKE) image
