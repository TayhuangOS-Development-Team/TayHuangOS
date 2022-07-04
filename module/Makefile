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
#	$(CD) drivers ; $(MAKE) build
	$(CD) setup ; $(MAKE) build
#	$(CD) test ; $(MAKE) build
#	$(CD) vdd ; $(MAKE) build

#清理
.PHONY: clean
clean:
#	$(CD) drivers ; $(MAKE) clean
	$(CD) setup ; $(MAKE) clean
#	$(CD) test ; $(MAKE) clean
#	$(CD) vdd ; $(MAKE) clean

.PHONY: image
image:
#	$(CD) drivers ; $(MAKE) image
	$(CD) setup ; $(MAKE) image
#	$(CD) test ; $(MAKE) image
#	$(CD) vdd ; $(MAKE) image