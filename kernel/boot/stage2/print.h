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

PUBLIC void printChar(char ch);
PUBLIC void printStr(const char *str);
PUBLIC void printReturn(void);
PUBLIC void printByte(byte num, bool is_signed);
PUBLIC void printByteHex(byte num, bool disp_0x);
PUBLIC void printShort(word num, bool is_signed);
PUBLIC void printShortHex(word num, bool disp_0x);
PUBLIC void printInt(dword num, bool is_signed);
PUBLIC void printIntHex(dword num, bool disp_0x);
PUBLIC void changePos(word x, word y);
PUBLIC word getPosX(void);
PUBLIC word getPosY(void);

PUBLIC void dispChar(char ch, byte color, byte x, byte y);
PUBLIC void dispStr(const char *str, byte color, byte x, byte y);
PUBLIC void dispByte(byte num, byte color, byte x, byte y, bool is_signed);
PUBLIC void dispByteHex(byte num, byte color, byte x, byte y, bool disp_0x);
PUBLIC void dispShort(word num, byte color, byte x, byte y, bool is_signed);
PUBLIC void dispShortHex(word num, byte color, byte x, byte y, bool disp_0x);
PUBLIC void dispInt(dword num, byte color, byte x, byte y, bool is_signed);
PUBLIC void dispIntHex(dword num, byte color, byte x, byte y, bool disp_0x);

PUBLIC void clrscr(void);