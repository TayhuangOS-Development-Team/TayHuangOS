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

#define KHEAP_BASE (0x100000)

static inline void memset(_OUT void *dst, _IN byte val, _IN int sz) {
    for (int i = 0 ; i < sz ; i ++) {
        *(char*)(dst + i) = val;
    }
}

static inline void memcpy(_OUT void *dst, _IN void *src, _IN int sz) {
    for (int i = 0 ; i < sz ; i ++) {
        *(char*)(dst + i) = *(char*)(src + i);
    }
}

PUBLIC void init_kheap(_IN void *kheap_limit);
PUBLIC void *malloc(_IN int size);
static inline void *calloc(_IN int num, _IN int size) {
    return malloc(num * size);
}
PUBLIC void free(_IN void *ptr);