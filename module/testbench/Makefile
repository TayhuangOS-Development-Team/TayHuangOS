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



OBJECTSDIR := $(OBJECTSDIR)/module/
BINDIR := $(BINDIR)/module/
TESTBENCHDIR := $(MODULEDIR)/testbench/
export OBJECTSDIR BINDIR TESTBENCHDIR

#编译
.PHONY: build
build:
	$(CD) testbench1 && $(MAKE) build
	$(CD) testbench2 && $(MAKE) build

#清理
.PHONY: clean
clean:
	$(CD) testbench1 && $(MAKE) clean
	$(CD) testbench2 && $(MAKE) clean

.PHONY: image
image:
	$(CD) testbench1 && $(MAKE) image
	$(CD) testbench2 && $(MAKE) image