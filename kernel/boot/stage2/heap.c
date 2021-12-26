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
#include "drivers/devices.h"
#include "drivers/memory/memory_driver.h"

PRIVATE struct {
    sreg_t heap_seg;
    addr_t heap_bottom;
    addr_t heap_top;
    dword heap_sz;
} LL_HEAP;

#define LL_HEAP_SEGMENT 0x0700
#define LL_HEAP_SZ 0x0100

PUBLIC void ll_init_heap(void) {
    LL_HEAP.heap_seg = LL_HEAP_SEGMENT;
    LL_HEAP.heap_top = LL_HEAP.heap_bottom = 0x0000;
    LL_HEAP.heap_seg = LL_HEAP_SZ;
}

PUBLIC void ll_resize_heap(word sz) {
    LL_HEAP.heap_sz = LL_HEAP_SZ;
}

PUBLIC word ll_get_heap_used(void) {
    return LL_HEAP.heap_top - LL_HEAP.heap_bottom;
}

PUBLIC word ll_get_heap_remain(void) {
    return LL_HEAP.heap_sz - ll_get_heap_used();
}

PUBLIC addr_t ll_alloc(word sz) {
    addr_t point = LL_HEAP.heap_top;
    if (ll_get_heap_remain() < sz) {
        ll_resize_heap(max (LL_HEAP.heap_sz * 2, LL_HEAP.heap_sz + sz + 1));
    }
    LL_HEAP.heap_top += sz;
    return point;
}

PUBLIC void ll_free(addr_t addr) {
}

PUBLIC byte ll_get_byte(addr_t addr) {
    sreg_t essv = rdes();
    stes(LL_HEAP.heap_seg);
    byte res = rdes8(addr);
    stes(essv);
    return res;
}

PUBLIC word ll_get_word(addr_t addr) {
    sreg_t essv = rdes();
    stes(LL_HEAP.heap_seg);
    word res = rdes16(addr);
    stes(essv);
    return res;
}

PUBLIC dword ll_get_dword(addr_t addr) {
    sreg_t essv = rdes();
    stes(LL_HEAP.heap_seg);
    dword res = rdes32(addr);
    stes(essv);
    return res;
}

PUBLIC void ll_set_byte(addr_t addr, byte val) {
    sreg_t essv = rdes();
    stes(LL_HEAP.heap_seg);
    stes8(addr, val);
    stes(essv);
}

PUBLIC void ll_set_word(addr_t addr, word val) {
    sreg_t essv = rdes();
    stes(LL_HEAP.heap_seg);
    stes16(addr, val);
    stes(essv);
}

PUBLIC void ll_set_dword(addr_t addr, dword val) {
    sreg_t essv = rdes();
    stes(LL_HEAP.heap_seg);
    stes32(addr, val);
    stes(essv);
}

PUBLIC void ll_cp_from_heap(addr_t src, void* dst, word num) {
    for (word i = 0 ; i < num ; i ++) {
        *((byte*)dst + i) = ll_get_byte(src + i);
    }
}

PUBLIC void ll_cp_to_heap(void* src, addr_t dst, word num) {
    for (word i = 0 ; i < num ; i ++) {
        ll_set_byte(src + i, *((byte*)dst + i));
    }
}

PUBLIC void ll_cp_heap_to_heap(addr_t src, addr_t dst, word num) {
    for (word i = 0 ; i < num ; i ++) {
        ll_set_byte(src + i, ll_get_byte(dst + i));
    }
}

PUBLIC sreg_t ll_get_heap_seg(void) {
    return LL_HEAP.heap_seg;
}

PUBLIC void init_heap(void) {
    ((pdriver)memory_device.driver)->pc_handle(&memory_device, memory_device.driver, MM_CMD_INIT_HEAP, NULL);
}

PUBLIC dword get_heap_used(void) {
    dword result;
    ((pdriver)memory_device.driver)->pc_handle(&memory_device, memory_device.driver, MM_CMD_GET_USED, &result);
    return result;
}

PUBLIC dword get_heap_remain(void) {
    dword result;
    ((pdriver)memory_device.driver)->pc_handle(&memory_device, memory_device.driver, MM_CMD_GET_REMAIN, &result);
    return result;
}

PRIVATE APACK(mm, alloc) alloc_pack;

PUBLIC addr_t alloc(dword sz, bool weak) {
    alloc_pack.length = sz;
    alloc_pack.weak = weak;
    ((pdriver)memory_device.driver)->pc_handle(&memory_device, memory_device.driver, MM_CMD_ALLOC, &alloc_pack);
    return alloc_pack.address;
}

PUBLIC void free(addr_t addr) {
    ((pdriver)memory_device.driver)->pc_handle(&memory_device, memory_device.driver, MM_CMD_FREE, &addr);
}

PRIVATE APACK(mm, get_data) get_data_pack;

PUBLIC byte get_heap_byte(addr_t addr) {
    byte result;
    get_data_pack.src = addr;
    get_data_pack.dst = &result;
    get_data_pack.len = sizeof(result);
    ((pdriver)memory_device.driver)->pc_handle(&memory_device, memory_device.driver, MM_CMD_GET_DATA, &get_data_pack);
    return result;
}

PUBLIC word get_heap_word(addr_t addr) {
    word result;
    get_data_pack.src = addr;
    get_data_pack.dst = &result;
    get_data_pack.len = sizeof(result);
    ((pdriver)memory_device.driver)->pc_handle(&memory_device, memory_device.driver, MM_CMD_GET_DATA, &get_data_pack);
    return result;
}

PUBLIC dword get_heap_dword(addr_t addr) {
    dword result;
    get_data_pack.src = addr;
    get_data_pack.dst = &result;
    get_data_pack.len = sizeof(result);
    ((pdriver)memory_device.driver)->pc_handle(&memory_device, memory_device.driver, MM_CMD_GET_DATA, &get_data_pack);
    return result;
}

PRIVATE APACK(mm, set_data) set_data_pack;

PUBLIC void set_heap_byte(addr_t addr, byte val) {
    set_data_pack.src = addr;
    set_data_pack.dst = &val;
    set_data_pack.len = sizeof(val);
    ((pdriver)memory_device.driver)->pc_handle(&memory_device, memory_device.driver, MM_CMD_SET_DATA, &set_data_pack);
}

PUBLIC void set_heap_word(addr_t addr, word val) {
    set_data_pack.src = addr;
    set_data_pack.dst = &val;
    set_data_pack.len = sizeof(val);
    ((pdriver)memory_device.driver)->pc_handle(&memory_device, memory_device.driver, MM_CMD_SET_DATA, &set_data_pack);
}

PUBLIC void set_heap_dword(addr_t addr, dword val) {
    set_data_pack.src = addr;
    set_data_pack.dst = &val;
    set_data_pack.len = sizeof(val);
    ((pdriver)memory_device.driver)->pc_handle(&memory_device, memory_device.driver, MM_CMD_SET_DATA, &set_data_pack);
}

PUBLIC void cp_from_heap(addr_t src, void* dst, word num) {
    get_data_pack.src = src;
    get_data_pack.dst = dst;
    get_data_pack.len = num;
    ((pdriver)memory_device.driver)->pc_handle(&memory_device, memory_device.driver, MM_CMD_GET_DATA, &get_data_pack);
}

PUBLIC void cp_to_heap(void* src, addr_t dst, word num) {
    set_data_pack.src = src;
    set_data_pack.dst = dst;
    set_data_pack.len = num;
    ((pdriver)memory_device.driver)->pc_handle(&memory_device, memory_device.driver, MM_CMD_SET_DATA, &set_data_pack);
}

PRIVATE APACK(mm, cp_data) cp_data_pack;

PUBLIC void cp_heap_to_heap(addr_t src, addr_t dst, word num) {
    cp_data_pack.src = src;
    cp_data_pack.dst = dst;
    cp_data_pack.len = num;
    ((pdriver)memory_device.driver)->pc_handle(&memory_device, memory_device.driver, MM_CMD_CP_DATA, &cp_data_pack);
}

PUBLIC sreg_t get_heap_seg(void) {
    sreg_t segment;
    ((pdriver)memory_device.driver)->pc_handle(&memory_device, memory_device.driver, MM_CMD_GET_HEAP_SEGMENT, &segment);
    return segment;
}

PUBLIC void update_heap(void) {
    ((pdriver)memory_device.driver)->pc_handle(&memory_device, memory_device.driver, MM_CMD_UPDATE, NULL);
}