// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/heap.c
 *
 * Heap manage function is implemented here
 */



#include "heap.h"

PRIVATE struct {
    sreg_t heap_seg;
    addr_t heap_bottom;
    addr_t heap_top;
    dword heap_sz;
    enum {
        HPC_NONE = 0,
        HPC_GLOBAL,
        HPC_DRIVER
    } controller;
} HEAP;

#define LL_HEAP_SEGMENT 0x0700
#define LL_HEAP_SZ 0x0100

PUBLIC void ll_init_heap(void) {
    HEAP.controller = HPC_GLOBAL;
    HEAP.heap_seg = LL_HEAP_SEGMENT;
    HEAP.heap_top = HEAP.heap_bottom = 0x0000;
    HEAP.heap_seg = LL_HEAP_SZ;
}

PUBLIC void ll_resize_heap(word sz) {
    HEAP.heap_sz = LL_HEAP_SZ;
}

PUBLIC word ll_get_heap_used(void) {
    return HEAP.heap_top - HEAP.heap_bottom;
}

PUBLIC word ll_get_heap_remain(void) {
    return HEAP.heap_sz - ll_get_heap_used();
}

PUBLIC addr_t ll_alloc(word sz) {
    addr_t point = HEAP.heap_top;
    if (ll_get_heap_remain() < sz) {
        ll_resize_heap(max (HEAP.heap_sz * 2, HEAP.heap_sz + sz + 1));
    }
    HEAP.heap_top += sz;
    return point;
}

PUBLIC void ll_free(addr_t addr) {
}

PUBLIC byte ll_get_byte(addr_t addr) {
    sreg_t essv = rdes();
    stes(HEAP.heap_seg);
    byte res = rdes8(addr);
    stes(essv);
    return res;
}

PUBLIC word ll_get_word(addr_t addr) {
    sreg_t essv = rdes();
    stes(HEAP.heap_seg);
    word res = rdes16(addr);
    stes(essv);
    return res;
}

PUBLIC dword ll_get_dword(addr_t addr) {
    sreg_t essv = rdes();
    stes(HEAP.heap_seg);
    dword res = rdes32(addr);
    stes(essv);
    return res;
}

PUBLIC void ll_set_byte(addr_t addr, byte val) {
    sreg_t essv = rdes();
    stes(HEAP.heap_seg);
    stes8(addr, val);
    stes(essv);
}

PUBLIC void ll_set_word(addr_t addr, word val) {
    sreg_t essv = rdes();
    stes(HEAP.heap_seg);
    stes16(addr, val);
    stes(essv);
}

PUBLIC void ll_set_dword(addr_t addr, dword val) {
    sreg_t essv = rdes();
    stes(HEAP.heap_seg);
    stes32(addr, val);
    stes(essv);
}

PUBLIC void ll_cp_bytes(addr_t src, addr_t dst, word num) {
    for (word i = 0 ; i < num ; i ++) {
        *((byte*)dst + i) = ll_get_byte(src + i);
    }
}

PUBLIC void ll_cp_from(addr_t src, addr_t dst, word num) {
    for (word i = 0 ; i < num ; i ++) {
        ll_set_byte(src + i, *((byte*)dst + i));
    }
}