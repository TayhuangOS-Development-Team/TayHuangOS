#!/usr/bin/python3 
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
# counter.py
# 
# 构建计数器
# 
#



build_file = open("times.txt", mode='wb+')

build_times = int(build_file.read())
build_times = build_times + 1

build_file.write(str(build))