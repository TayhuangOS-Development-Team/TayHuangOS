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

#define KHEAP_BASE (0x00001)
#define KHEAP_LIMIT (0x9FFFF)

PUBLIC void memset(_OUT void *dst, _IN byte val, _IN int sz);
PUBLIC void memcpy(_OUT void *dst, _IN void *src, _IN int sz);
PUBLIC void init_kheap(void);
PUBLIC void *malloc(_IN int size);
static inline void *calloc(_IN int num, _IN int size) {
    return malloc(num * size);
}
PUBLIC void free(_IN void *ptr);