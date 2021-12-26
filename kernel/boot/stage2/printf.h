// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong Beanflame
 *
 * kernel/boot/stage2/printf.h
 *
 * Display functions are declared here
 */



#pragma once

#include "header.h"

PUBLIC void disp_char(char ch, byte color, byte x, byte y);
PUBLIC void change_pos(word x, word y);
PUBLIC word get_pos_x(void);
PUBLIC word get_pos_y(void);
PUBLIC void putchar(char ch);
PUBLIC void puts(const char* str);

PUBLIC void clrscr(void);

PUBLIC int vsprintf(char* buffer, const char* format, ...);
PUBLIC int printf(const char* format, ...);