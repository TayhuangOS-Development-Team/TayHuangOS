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
 * kheap.c
 * 
 * 内核堆
 * 
 */



#include "kheap.h"

PUBLIC void memset(void *dst, byte val, int sz) {
    for (int i = 0 ; i < sz ; i ++) {
        *(char*)(dst + i) = val;
    }
}

PUBLIC void memcpy(void *dst, void *src, int sz) {
    for (int i = 0 ; i < sz ; i ++) {
        *(char*)(dst + i) = *(char*)(src + i);
    }
}

typedef struct {
    void *start;
    void *limit;
} kh_seg;

#define KH_SEG_NUM (8192)
PRIVATE kh_seg *KHEAP_SEGMENTS = (kh_seg*)(KHEAP_LIMIT - sizeof(kh_seg) * KH_SEG_NUM);
PRIVATE void *KHEAP_TOP = (void*)(KHEAP_LIMIT - sizeof(kh_seg) * KH_SEG_NUM);
PRIVATE void *KHEAP_BOTTOM = (void*)KHEAP_BASE;

PUBLIC void init_kheap(void) {
    KHEAP_SEGMENTS = (kh_seg*)(KHEAP_LIMIT - sizeof(kh_seg) * KH_SEG_NUM);
    KHEAP_TOP = (void*)(KHEAP_LIMIT - sizeof(kh_seg) * KH_SEG_NUM);
    KHEAP_BOTTOM = (void*)KHEAP_BASE;
    memset(KHEAP_SEGMENTS, 0, sizeof(kh_seg) * KH_SEG_NUM);
}

PRIVATE void *__query_using(void *base, int size) {
    for (int i = 0 ; i < KH_SEG_NUM ; i ++) {
        if (max(KHEAP_SEGMENTS[i].start, base) >= min(KHEAP_SEGMENTS[i].limit, (base + size))) {
            return KHEAP_SEGMENTS[i].limit + 1;
        }
    }
    return NULL;
}

PRIVATE void *__lookup_free_mem(int size) {
    for (void *i = KHEAP_BOTTOM ; i < KHEAP_TOP ;) {
        void *result = __query_using(i, size);
        if (result == NULL) {
            return i;
        }
        i = result;
    }
    return NULL;
}

PRIVATE int __lookup_free_kh_seg(void) {
    for (int i = 0 ; i < KH_SEG_NUM ; i ++) {
        if ((KHEAP_SEGMENTS[i].start == NULL) && (KHEAP_SEGMENTS[i].limit == NULL)) {
            return i;
        }
    }
    return -1;
}

PRIVATE bool __insert_kh_seg(void *start, void *limit) {
    int idx = __lookup_free_kh_seg();
    if (idx == -1) {
        return false;
    }
    KHEAP_SEGMENTS[idx].start = start;
    KHEAP_SEGMENTS[idx].limit = limit;
    return true;
}

PRIVATE void __delete_kh_seg(void *start) {
    for (int i = 0 ; i < KH_SEG_NUM ; i ++) {
        if ((KHEAP_SEGMENTS[i].start == start)) {
            KHEAP_SEGMENTS[i].start = NULL;
            KHEAP_SEGMENTS[i].limit = NULL;
            break;
        }
    }
}

PUBLIC void *malloc(int size) {
    void *mem = __lookup_free_mem(size);
    if (mem == NULL)
        return NULL;
    if (! __insert_kh_seg(mem, mem + size))
        return NULL;
    return mem;
}

PUBLIC void free(void *ptr) {
    __delete_kh_seg(ptr);
}