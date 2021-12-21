// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong Beanflame
 *
 * kernel/boot/stage2/print.h
 *
 * Display functions are declared here
 */



#pragma once
#include "header.h"

PUBLIC void print_char(char ch);
PUBLIC void print_string(const char *str);
PUBLIC void print_return(void);
PUBLIC void print_byte(byte num, bool is_signed);
PUBLIC void print_byte_hex(byte num, bool disp_0x);
PUBLIC void print_short(word num, bool is_signed);
PUBLIC void print_short_hex(word num, bool disp_0x);
PUBLIC void print_int(dword num, bool is_signed);
PUBLIC void print_int_hex(dword num, bool disp_0x);
PUBLIC void change_pos(word x, word y);
PUBLIC word get_pos_x(void);
PUBLIC word get_pos_y(void);

PUBLIC void disp_char(char ch, byte color, byte x, byte y);
PUBLIC void disp_string(const char *str, byte color, byte x, byte y);
PUBLIC void disp_byte(byte num, byte color, byte x, byte y, bool is_signed);
PUBLIC void disp_byte_hex(byte num, byte color, byte x, byte y, bool disp_0x);
PUBLIC void disp_short(word num, byte color, byte x, byte y, bool is_signed);
PUBLIC void disp_short_hex(word num, byte color, byte x, byte y, bool disp_0x);
PUBLIC void disp_int(dword num, byte color, byte x, byte y, bool is_signed);
PUBLIC void disp_int_hex(dword num, byte color, byte x, byte y, bool disp_0x);

PUBLIC void clrscr(void);

PUBLIC void vsprintf(char* buffer, const char* format, ...);
PUBLIC void printf(const char* format, ...);