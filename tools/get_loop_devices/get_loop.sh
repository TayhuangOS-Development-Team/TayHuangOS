#!/bin/bash
#
# SPDX-License-Identifier: LGPL-2.1-only
# -------------------------------*-TayhuangOS-*-----------------------------------
# 
#              Copyright (C) 2022, 2022 TayhuangOS Development Team
# 
# --------------------------------------------------------------------------------
# 
# 作者: 383494
# 
# get_loop.sh
# 
# 检测当前可用的回环设备
#
# 输入(stdin)：$1 前一个回环设备（默认0）
# 输出(stdout)：当前第一个可用的回环设备
# 例： 	get_loop.sh 0 			输出 /dev/loop1
# 	get_loop.sh /dev/loop0 		输出 /dev/loop1
#	get_loop.sh 1(/dev/loop2不可用)	输出 /dev/loop3
#


loop=$(echo $1 | tr -cd "[0-9]")
let loop++
while [[ -n $(losetup -a | grep "loop"$loop":") ]]
do
	let loop++
done
echo "/dev/loop"$loop
