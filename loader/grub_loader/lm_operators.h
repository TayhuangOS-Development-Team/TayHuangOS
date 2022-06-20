/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * setup_longmode.h
 *
 * 设置长模式
 *
 */



#pragma once

#include <tayhuang/defs.h>

PUBLIC void set_cr0(int cr0);
PUBLIC int get_cr0(void);
PUBLIC void set_cr2(int cr2);
PUBLIC int get_cr2(void);
PUBLIC void set_cr3(int cr3);
PUBLIC int get_cr3(void);
PUBLIC void set_cr4(int cr4);
PUBLIC int get_cr4(void);
PUBLIC void set_efer(int efer);
PUBLIC int get_efer(void);
PUBLIC void flush_cs(short cs);