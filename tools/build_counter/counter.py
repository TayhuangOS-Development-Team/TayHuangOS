#!/usr/bin/python3 
#
# SPDX-License-Identifier: LGPL-2.1-only
# -------------------------------*-TayhuangOS-*-----------------------------------
# 
#              Copyright (C) 2022, 2022 TayhuangOS Development Team
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



import pathlib

build_times_file = "./configs/times.txt"

build_times = 0

if pathlib.Path(build_times_file).exists():
    build_file = open(build_times_file, mode='r')

    build_times = int(build_file.read(-1))

    build_file.close()

build_file = open(build_times_file, mode='w+')

build_times = build_times + 1

build_file.write(str(build_times))

build_file.close()