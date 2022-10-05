/* 
 * SPDX-License-Identifier: LGPL-2.1-only
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
PUBLIC void setup_longmode(NONNULL void *pml4);
PUBLIC void NULLABLE("couldn't setup paging") *setup_paging(qword memsz, NONNULL void ** limit);

PUBLIC void goto_longmode(word selector64, qword memsz, bool is_graphic, int screen_width, int screen_height, NONNULL void *framebuffer);