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

#include <tayhuang/defs.h>
#include <stdarg.h>

//获取/设置输出颜色
PUBLIC int get_print_color(void);
PUBLIC void set_print_color(int color);
//设置tty
PUBLIC int get_tty(void);
PUBLIC void set_tty(int tty);
PUBLIC void set_active_tty(int tty);
//获取/改变输出位置
PUBLIC void change_pos(int x, int y);
PUBLIC int get_pos_x(void);
PUBLIC int get_pos_y(void);
//获取/改变滚动行位置
PUBLIC int get_scroll_line(void);
PUBLIC void set_scroll_line(int line);
//滚动屏幕
PUBLIC void scroll_screen(int lines);
//刷新到屏幕上
PUBLIC void flush_to_screen(void);

//清屏
PUBLIC void clrscr(void);
//打印字符(串)
PUBLIC void putchar(char ch);
PUBLIC void puts(const char *str);
//printf变体
PUBLIC int vsprintf(char *buffer, const char *format, va_list args);
PUBLIC int printf(const char *format, ...);
PUBLIC int sprintf(char *buffer, const char *format, ...);