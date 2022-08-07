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
 * __tty_driver_fn.h
 * 
 * __tty_driver_fn 函数功能
 * 
 */



#pragma once

#include <tayhuang/defs.h>

#define PUTCHAR_FN (0)
#define PUTCHAR_ARGS_SIZE (sizeof(int) + sizeof(char))
#define PUTCHAR_RETURN_TYPE bool

#define FLUSH_CONSOLE_FN (1)
#define FLUSH_CONSOLE_ARGS_SIZE (sizeof(int))
#define FLUSH_CONSOLE_RETURN_TYPE bool

#define PUTS_FN (2)
#define PUTS_ARGS_SIZE (-1)
#define PUTS_ARGS_BASE_SIZE (sizeof(int) + sizeof(int))
#define PUTS_RETURN_TYPE bool

#define GETCHAR_FN (3)
#define GETCHAR_ARGS_SIZE (sizeof(int))
#define GETCHAR_RETURN_TYPE char

PUBLIC void flush_console(int console_no);
PUBLIC bool tty_putchar(int console_no, char ch);
PUBLIC bool tty_puts(int console_no, const char *str);
PUBLIC char tty_getchar(int console_no);