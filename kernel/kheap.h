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
 * kheap.h
 * 
 * 内核堆
 * 
 */



#pragma once

#include <tayhuang/defs.h>

#define KHEAP_BASE (0x00000)
#define KHEAP_LIMIT (0x9FFFF)

PUBLIC void memset(void *dst, byte val, int sz);
PUBLIC void memcpy(void *dst, void *src, int sz);
PUBLIC void init_kheap(void);
PUBLIC void *malloc(int size);
static inline void *calloc(int num, int size) {
    return malloc(num * size);
}
PUBLIC void free(void *ptr);