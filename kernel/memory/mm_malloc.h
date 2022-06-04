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
 * mm_malloc.h
 * 
 * malloc系函数
 * 
 */



#pragma once

#include <tayhuang/defs.h>

PUBLIC void *cpmalloc(int size); //size向上取页大小的倍数 连续物理空间
static inline void *cpcalloc(int size, int num) {
    return cpmalloc(size * num);
}
PUBLIC void cpfree(void *addr, int size);