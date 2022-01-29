// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
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
#include "file.h"

struct screen_info_t{
    int width;
    int height;
    int current_line;
    int scroll_line;
};

extern struct screen_info_t screen_info;

PUBLIC void disp_char(char ch, byte color, byte x, byte y);
PUBLIC void change_pos(word x, word y);
PUBLIC word get_pos_x(void);
PUBLIC word get_pos_y(void);
PUBLIC void putchar(char ch);
PUBLIC void puts(const char* str);

PUBLIC void clrscr(void);
PUBLIC byte get_print_color(void);
PUBLIC void set_print_color(byte color);

PUBLIC int vsprintf(char* buffer, const char* format, ...);
PUBLIC int printf(const char* format, ...);
PUBLIC int fprintf(FILE *file, const char *format, ...);

PUBLIC void fputc(FILE *file, char ch);
PUBLIC void scroll_screen(int offset);