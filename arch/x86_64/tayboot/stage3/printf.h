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
 * arch/x86_64/tayboot/stage3/printf.h
 *
 * 打印函数
 */



#pragma once

#include <tayboot/tay_defs.h>

struct print_info_t {
    int scroll_line;
};

extern struct print_info_t print_info;

PUBLIC void change_pos(word x, word y);
PUBLIC word get_pos_x(void);
PUBLIC word get_pos_y(void);
PUBLIC void putchar(char ch);
PUBLIC void puts(const char *str);

PUBLIC void clrscr(void);
PUBLIC byte get_print_color(void);
PUBLIC void set_print_color(byte color);

PUBLIC int vsprintf(char *buffer, const char *format, ...);
PUBLIC int printf(const char *format, ...);
PUBLIC void scroll_screen(int num);