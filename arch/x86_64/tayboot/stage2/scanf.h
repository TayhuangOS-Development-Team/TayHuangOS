/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage2/scanf.h
 *
 * Input functions are declared here
 */



#pragma once

#include "header.h"
#include "file.h"

PUBLIC int getkey(void); //获取击键
PUBLIC int getchar(void); //取字符
PUBLIC void backchar(int ch); //退字符
PUBLIC int scanf(const char *format, ...);
typedef int(*rdkey_t)(void);
typedef void(*bkkey_t)(int);
PUBLIC int vscanf(bkkey_t bk_func, rdkey_t rd_func, const char *format, ...); //从给定的bk_func和rd_func操作流
PUBLIC int fscanf(FILE *file, const char *format, ...); //读取文件
PUBLIC void init_key_buffer(void);
PUBLIC void clear_buffer(void);
PUBLIC int fgetc(FILE *file); //读入文件中的一个字符
PUBLIC void getline(char *str); //读取一行
PUBLIC void fgetline(FILE *file, char *str); //从文件中读取一行