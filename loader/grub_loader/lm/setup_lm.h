/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: theflysong
 *
 * setup_lm.h
 *
 * 设置长模式
 *
 */



#pragma once

#include <tayhuang/defs.h>

#define PAGE_ADDRESS (0x3000000)
PUBLIC void setup_longmode(void *pml4);
PUBLIC void *setup_paging(dword memsz, dword memsz_high, void** limit);

PUBLIC void goto_longmode(word selector64, dword memsz, dword memsz_high, bool is_graphic, int screen_width, int screen_height, void *framebuffer);