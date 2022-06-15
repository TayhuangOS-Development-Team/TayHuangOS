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
 * keyboard.h
 * 
 * 键盘
 * 
 */



#pragma once

#include <intterup/init_int.h>

PUBLIC void init_keyboard(void);
PUBLIC void deal_key(void);
PUBLIC char getchar(void);
PUBLIC short keyboard_int_handler(int irq, struct intterup_args *regs, bool entered_handler);
PUBLIC void keyboard_handler(void);
PUBLIC void keyboard_api_process(void);