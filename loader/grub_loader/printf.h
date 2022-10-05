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
 * printf.h
 *
 * printf
 *
 */



#pragma once

#include <stdarg.h>

#include <tayhuang/defs.h>

#define VIDEO_ADDRESS (0xB8000) //显存地址
#define SCREEN_WIDTH (80)
#define SCREEN_HEIGHT (25)

//写生字符(串)
PUBLIC void write_char(char ch, int color, int posx, int posy);
PUBLIC void write_str(NONNULL const char *str, int color, int posx, int posy);

PUBLIC void change_pos(int x, int y);
PUBLIC int get_pos_x(void);
PUBLIC int get_pos_y(void);
PUBLIC void putchar(char ch);
PUBLIC void puts(NONNULL const char *str);

PUBLIC void clrscr(void);
PUBLIC int get_print_color(void);
PUBLIC void set_print_color(int color);

PUBLIC int vsprintf(NONNULL char *buffer, NONNULL const char *format, va_list args);
PUBLIC int sprintf(NONNULL char *buffer, NONNULL const char *format, ...);
PUBLIC int printf(NONNULL const char *format, ...);