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



OBJECTSDIR := $(OBJECTSDIR)/libs/
export OBJECTSDIR

#编译
.PHONY: build
build:
	$(CD) libc && $(MAKE) build
	$(CD) libkmod && $(MAKE) build

#清理
.PHONY: clean
clean:
	$(CD) libc && $(MAKE) clean
	$(CD) libkmod && $(MAKE) clean