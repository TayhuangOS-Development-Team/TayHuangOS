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
# converter.py
# 
# 图片转换器
# 
#



import cv2
import sys

img = cv2.imread(sys.argv[1], flags = 1)
output = open(sys.argv[2], mode='wb+')

width = img.shape[0]
height = img.shape[1]

shrink = 2

output.write(int(width / shrink).to_bytes(4, 'little'))
output.write(int(height / shrink).to_bytes(4, 'little'))

for i in range(int(width / shrink)):
    for j in range(int(height / shrink)):
        red   = int(img[i * shrink, j * shrink, 2])
        green = int(img[i * shrink, j * shrink, 1])
        blue  = int(img[i * shrink, j * shrink, 0])
        output.write(red.to_bytes(1, 'little'))
        output.write(green.to_bytes(1, 'little'))
        output.write(blue.to_bytes(1, 'little'))
        output.write(int(0).to_bytes(1, 'little'))
