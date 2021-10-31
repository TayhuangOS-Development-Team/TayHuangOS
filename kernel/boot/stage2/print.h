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

void printchar(char ch);
void printstr(const char *str);
void printreturn(void);
void printbyte(byte num, bool is_signed);
void printbytehex(byte num, bool disp_0x);
void printshort(word num, bool is_signed);
void printshorthex(word num, bool disp_0x);
void printint(dword num, bool is_signed);
void printinthex(dword num, bool disp_0x);
void changepos(word x, word y);
word getposx(void);
word getposy(void);

void dispchar(char ch, byte color, byte x, byte y);
void dispstr(const char *str, byte color, byte x, byte y);
void dispbyte(byte num, byte color, byte x, byte y, bool is_signed);
void dispbytehex(byte num, byte color, byte x, byte y, bool disp_0x);
void dispshort(word num, byte color, byte x, byte y, bool is_signed);
void dispshorthex(word num, byte color, byte x, byte y, bool disp_0x);
void dispint(dword num, byte color, byte x, byte y, bool is_signed);
void dispinthex(dword num, byte color, byte x, byte y, bool disp_0x);

void clrscr(void);