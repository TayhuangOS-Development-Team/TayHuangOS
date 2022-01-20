// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/buffer.c
 *
 * Buffer manage functions are implemented here
 */



#include "buffer.h"
#include "drivers/drivers.h"
#include "drivers/memory/memory_driver.h"
#include "printf.h"

PRIVATE struct {
    sreg_t buffer_seg;
    addr_t buffer_bottom;
    addr_t buffer_top;
    dword buffer_sz;
} LL_BUFFER;

#define LL_BUFFER_SEGMENT 0x0700
#define LL_BUFFER_SZ 0x0100
//低级堆
PUBLIC void ll_init_buffer(void) {
    LL_BUFFER.buffer_seg = LL_BUFFER_SEGMENT;
    LL_BUFFER.buffer_top = LL_BUFFER.buffer_bottom = 0x0000;
    LL_BUFFER.buffer_seg = LL_BUFFER_SZ;
}

PUBLIC void ll_resize_buffer(word sz) {
    LL_BUFFER.buffer_sz = LL_BUFFER_SZ;
}

PUBLIC word ll_get_buffer_used(void) {
    return LL_BUFFER.buffer_top - LL_BUFFER.buffer_bottom;
}

PUBLIC word ll_get_buffer_remain(void) {
    return LL_BUFFER.buffer_sz - ll_get_buffer_used();
}

PUBLIC addr_t ll_alloc_buffer(word sz) {
    addr_t point = LL_BUFFER.buffer_top;
    if (ll_get_buffer_remain() < sz) {
        ll_resize_buffer(max (LL_BUFFER.buffer_sz * 2, LL_BUFFER.buffer_sz + sz + 1));
    }
    LL_BUFFER.buffer_top += sz;
    return point;
}

PUBLIC void ll_free_buffer(addr_t addr) {
}

PUBLIC byte ll_get_byte_buffer(addr_t addr) {
    sreg_t essv = rdes();
    stes(LL_BUFFER.buffer_seg);
    byte res = rdes8(addr);
    stes(essv);
    return res;
}

PUBLIC word ll_get_word_buffer(addr_t addr) {
    sreg_t essv = rdes();
    stes(LL_BUFFER.buffer_seg);
    word res = rdes16(addr);
    stes(essv);
    return res;
}

PUBLIC dword ll_get_dword_buffer(addr_t addr) {
    sreg_t essv = rdes();
    stes(LL_BUFFER.buffer_seg);
    dword res = rdes32(addr);
    stes(essv);
    return res;
}

PUBLIC void ll_set_byte_buffer(addr_t addr, byte val) {
    sreg_t essv = rdes();
    stes(LL_BUFFER.buffer_seg);
    stes8(addr, val);
    stes(essv);
}

PUBLIC void ll_set_word_buffer(addr_t addr, word val) {
    sreg_t essv = rdes();
    stes(LL_BUFFER.buffer_seg);
    stes16(addr, val);
    stes(essv);
}

PUBLIC void ll_set_dword_buffer(addr_t addr, dword val) {
    sreg_t essv = rdes();
    stes(LL_BUFFER.buffer_seg);
    stes32(addr, val);
    stes(essv);
}

PUBLIC void ll_cp_from_buffer(addr_t src, void* dst, word num) {
    for (word i = 0 ; i < num ; i ++) {
        *((byte*)dst + i) = ll_get_byte_buffer(src + i);
    }
}

PUBLIC void ll_cp_to_buffer(void* src, addr_t dst, word num) {
    for (word i = 0 ; i < num ; i ++) {
        ll_set_byte_buffer(dst + i, *((byte*)src + i));
    }
}

PUBLIC void ll_cp_buffer_to_buffer(addr_t src, addr_t dst, word num) {
    for (word i = 0 ; i < num ; i ++) {
        ll_set_byte_buffer(dst + i, ll_get_byte_buffer(src + i));
    }
}

PUBLIC sreg_t ll_get_buffer_seg(void) {
    return LL_BUFFER.buffer_seg;
}

//高级的利用驱动完成的堆
PUBLIC void init_buffer(void) {
    memory_driver.pc_handle(&memory_driver, MM_CMD_INIT_BUFFER, NULL);
}

PUBLIC dword get_buffer_used(void) {
    dword result;
    memory_driver.pc_handle(&memory_driver, MM_CMD_GET_USED, &result);
    return result;
}

PUBLIC dword get_buffer_remain(void) {
    dword result;
    memory_driver.pc_handle(&memory_driver, MM_CMD_GET_REMAIN, &result);
    return result;
}

PRIVATE APACK(mm, alloc_buffer) alloc_pack;

PUBLIC addr_t alloc_buffer(dword sz, bool weak) {
    addr_t addr;
    alloc_pack.length = sz;
    alloc_pack.weak = weak;
    alloc_pack.address = &addr;
    memory_driver.pc_handle(&memory_driver, MM_CMD_ALLOC, &alloc_pack);
    return addr;
}

PUBLIC void free_buffer(addr_t addr) {
    memory_driver.pc_handle(&memory_driver, MM_CMD_FREE, &addr);
}

PRIVATE APACK(mm, get_data) get_data_pack;

PUBLIC byte get_buffer_byte(addr_t addr) {
    byte result;
    get_data_pack.src = addr;
    get_data_pack.dst = &result;
    get_data_pack.size = sizeof(result);
    memory_driver.pc_handle(&memory_driver, MM_CMD_GET_DATA, &get_data_pack);
    return result;
}

PUBLIC word get_buffer_word(addr_t addr) {
    word result;
    get_data_pack.src = addr;
    get_data_pack.dst = &result;
    get_data_pack.size = sizeof(result);
    memory_driver.pc_handle(&memory_driver, MM_CMD_GET_DATA, &get_data_pack);
    return result;
}

PUBLIC dword get_buffer_dword(addr_t addr) {
    dword result;
    get_data_pack.src = addr;
    get_data_pack.dst = &result;
    get_data_pack.size = sizeof(result);
    memory_driver.pc_handle(&memory_driver, MM_CMD_GET_DATA, &get_data_pack);
    return result;
}

PRIVATE APACK(mm, set_data) set_data_pack;

PUBLIC void set_buffer_byte(addr_t addr, byte val) {
    set_data_pack.src = &val;
    set_data_pack.dst = addr;
    set_data_pack.size = sizeof(val);
    memory_driver.pc_handle(&memory_driver, MM_CMD_SET_DATA, &set_data_pack);
}

PUBLIC void set_buffer_word(addr_t addr, word val) {
    set_data_pack.src = &val;
    set_data_pack.dst = addr;
    set_data_pack.size = sizeof(val);
    memory_driver.pc_handle(&memory_driver, MM_CMD_SET_DATA, &set_data_pack);
}

PUBLIC void set_buffer_dword(addr_t addr, dword val) {
    set_data_pack.src = &val;
    set_data_pack.dst = addr;
    set_data_pack.size = sizeof(val);
    memory_driver.pc_handle(&memory_driver, MM_CMD_SET_DATA, &set_data_pack);
}

PUBLIC void cp_from_buffer(addr_t src, void* dst, word num) {
    get_data_pack.src = src;
    get_data_pack.dst = dst;
    get_data_pack.size = num;
    memory_driver.pc_handle(&memory_driver, MM_CMD_GET_DATA, &get_data_pack);
}

PUBLIC void cp_to_buffer(void* src, addr_t dst, word num) {
    set_data_pack.src = src;
    set_data_pack.dst = dst;
    set_data_pack.size = num;
    memory_driver.pc_handle(&memory_driver, MM_CMD_SET_DATA, &set_data_pack);
}

PRIVATE APACK(mm, cp_data) cp_data_pack;

PUBLIC void cp_buffer_to_buffer(addr_t src, addr_t dst, word num) {
    cp_data_pack.src = src;
    cp_data_pack.dst = dst;
    cp_data_pack.size = num;
    memory_driver.pc_handle(&memory_driver, MM_CMD_CP_DATA, &cp_data_pack);
}

PUBLIC sreg_t get_buffer_seg(void) {
    sreg_t segment;
    memory_driver.pc_handle(&memory_driver, MM_CMD_GET_BUFFER_SEGMENT, &segment);
    return segment;
}

PUBLIC void update_buffer(void) {
    memory_driver.pc_handle(&memory_driver, MM_CMD_UPDATE, NULL);
}