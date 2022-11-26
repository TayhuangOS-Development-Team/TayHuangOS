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
# calc.py
# 
# 魔数计算工具
# 
#

string = input()

base = 256
mod = 0xFFFFFFFF
bais = 0x3C94A896

magic = bais
for ch in string:
    magic = magic * base + ord(ch)
    magic %= mod

rmagic = bais
for ch in string[::-1]:
    rmagic = rmagic * base + ord(ch)
    rmagic %= mod

magic = (magic * rmagic) % mod
magic = magic * len(string) % mod

print ("0x%08X" % magic)