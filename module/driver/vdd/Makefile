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



OBJECTSDIR := $(OBJECTSDIR)/vdd/
BINDIR := $(BINDIR)/vdd/
VDDDIR := $(DRIVERDIR)/vdd/
export OBJECTSDIR BINDIR VDDDIR

#编译
.PHONY: build
build:
	$(CD) tty && $(MAKE) build

#清理
.PHONY: clean
clean:
	$(CD) tty && $(MAKE) clean

.PHONY: image
image:
	$(CD) tty && $(MAKE) image