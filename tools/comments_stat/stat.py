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
# comments_stat.py
# 
# 注释统计工具
# 
#



import os

look_dirs = ['./kernel/', './libs/', './loader/', './module/']
    
level = 0

def stat_comment(fp):
    global level
    file_names = fp.rsplit('.', maxsplit = 1)

    # 没有后缀名
    if len(file_names) != 2:
        return 0, 0

    # 不是c文件
    if (file_names[1] != 'c') & (file_names[1] != 'h'):
        return 0, 0

    comment_num = 0
    code_num = 0
    with open(fp, 'r') as f:
        for line in f:
            # 去掉空格
            line = line.strip()

            # 空行
            if not line:
                continue

            # 是//开头的注释
            if line.startswith('//'):
                comment_num = comment_num + 1
                continue

            #是代码
            code_num = code_num + 1

            if line.find('//') != -1:
                comment_num = comment_num + 1

    for i in range(level):
        print ('  ', end='')

    print (fp + " code=" + str(code_num) + " comment=" + str(comment_num) + " comment density=" + (str(comment_num / (comment_num + code_num) * 100)) + "%")
    return code_num, comment_num

def stat(look_dir):
    global level
    comment_num = 0
    code_num = 0
    for root, dirs, files in os.walk(look_dir):
        for f in files:
            level = level + 1
            fcode, fcomment = stat_comment(os.path.join(root, f))
            level = level - 1
            code_num += fcode
            comment_num += fcomment
        for d in dirs:
            level = level + 1
            dcode, dcomment = stat(os.path.join(root, d))
            level = level - 1
            code_num += dcode
            comment_num += dcomment

    for i in range(level):
        print (' ', end='')
    print (look_dir + " code=" + str(code_num) + " comment=" + str(comment_num) + " comment density=" + (str(comment_num / (comment_num + code_num) * 100)) + "%")
    return code_num, comment_num

    

for look_dir in look_dirs:
    stat(look_dir)