// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/scanf.h
 *
 * Input functions are declared here
 */



#pragma once

#include "header.h"
#include "file.h"

PUBLIC int getkey(void);
PUBLIC int getchar(void);
PUBLIC void backchar(int ch);
PUBLIC int scanf(const char* format, ...);
typedef int(*rdkey_t)(void);
typedef void(*bkkey_t)(int);
PUBLIC int vscanf(bkkey_t bk_func, rdkey_t rd_func, const char* format, ...);
PUBLIC int fscanf(FILE *file, const char *format, ...);
PUBLIC void init_key_buffer(void);
PUBLIC void clear_buffer(void);
PUBLIC int fgetc(FILE *file);
PUBLIC void getline(char* str);
PUBLIC void fgetline(FILE *file, char *str);