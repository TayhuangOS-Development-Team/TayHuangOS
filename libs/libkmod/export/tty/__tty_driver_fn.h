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

#define GET_CONSOLE_FN (0)
#define GET_CONSOLE_ARGS_SIZE (sizeof(int))
#define GET_CONSOLE_RETURN_TYPE void *

#define FLUSH_CONSOLE_FN (1)
#define FLUSH_CONSOLE_ARGS_SIZE (sizeof(int))
#define FLUSH_CONSOLE_RETURN_TYPE bool

PUBLIC void *get_console(int console_no);
PUBLIC void flush_console(int console_no);