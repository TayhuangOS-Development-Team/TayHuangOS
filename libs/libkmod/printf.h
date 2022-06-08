/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * printf.h
 *
 * printf
 *
 */



#pragma once

#include <tayhuang/defs.h>

PUBLIC int get_print_color(void);
PUBLIC void set_print_color(int color);
PUBLIC void change_pos(int x, int y);
PUBLIC void change_cursor_pos(int x, int y);
PUBLIC int get_pos_x(void);
PUBLIC int get_pos_y(void);
PUBLIC int get_scroll_line(void);
PUBLIC void set_scroll_line(int line);
PUBLIC void scroll_screen(int lines);
PUBLIC void flush_to_screen(void);

PUBLIC void clrscr(void);
PUBLIC void putchar(char ch);
PUBLIC void puts(const char *str);
PUBLIC int printf(const char *format, ...);
PUBLIC int sprintf(char *buffer, const char *format, ...);

PUBLIC void disable_cursor_move(void);
PUBLIC void enable_cursor_move(void);