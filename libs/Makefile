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
	$(CHANGE_DIR) libc ; $(MAKE) build
	$(CHANGE_DIR) libkmod ; $(MAKE) build
	$(CHANGE_DIR) libfifo ; $(MAKE) build

#清理
.PHONY: clean
clean:
	$(CHANGE_DIR) libc ; $(MAKE) clean
	$(CHANGE_DIR) libkmod ; $(MAKE) clean
	$(CHANGE_DIR) libfifo ; $(MAKE) clean