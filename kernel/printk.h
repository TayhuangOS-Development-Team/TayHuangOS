/*
 * SPDX-License-Identifier: LGPL-2.1-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: theflysong
 *
 * printk.h
 *
 * printk
 *
 */



#pragma once

#include <stdarg.h>

#include <tayhuang/defs.h>

PUBLIC void init_video(void *video_addr, int width, int height);

//写生字符(串)
PUBLIC void write_char(char ch, int color, int posx, int posy);
PUBLIC void write_str(const char *str, int color, int posx, int posy);

PUBLIC void change_pos(int x, int y);
PUBLIC int get_pos_x(void);
PUBLIC int get_pos_y(void);
PUBLIC void putchar(char ch);
PUBLIC void puts(const char *str);

PUBLIC void clrscr(void);
PUBLIC int get_print_color(void);
PUBLIC void set_print_color(int color);

PUBLIC int vsprintk(char *buffer, const char *format, va_list args);
PUBLIC int sprintk(char *buffer, const char *format, ...);
PUBLIC int printk(const char *format, ...);