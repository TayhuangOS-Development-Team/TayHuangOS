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
 * cmd.c
 *
 * tty 命令
 *
 */



#pragma once

#define TTY_PUTCHAR (0)
#define TTY_WRITE_STR (1)
#define TTY_CLEAR_SCREEN (2)
#define TTY_GETCHAR (3)
#define TTY_GETCHARS (4)
#define TTY_GETPOSX (5)
#define TTY_GETPOSY (6)
#define TTY_SETPOS (7)
#define TTY_SETACTIVE (8)
#define TTY_SETSCROLLLINE (9)
#define TTY_GETSCROLLLINE (10)