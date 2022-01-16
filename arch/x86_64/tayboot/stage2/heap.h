// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/heap.h
 *
 * Heap manage functions are declared here
 */



#pragma once

#include "header.h"

//Low level heap
PUBLIC void ll_init_heap(void);
PUBLIC void ll_resize_heap(word sz);
PUBLIC word ll_get_heap_used(void);
PUBLIC word ll_get_heap_remain(void);
PUBLIC addr_t ll_alloc(word sz);
PUBLIC void ll_free(addr_t addr);
PUBLIC byte ll_get_byte(addr_t addr);
PUBLIC word ll_get_word(addr_t addr);
PUBLIC dword ll_get_dword(addr_t addr);
PUBLIC void ll_set_byte(addr_t addr, byte val);
PUBLIC void ll_set_word(addr_t addr, word val);
PUBLIC void ll_set_dword(addr_t addr, dword val);
PUBLIC void ll_cp_from_heap(addr_t src, void* dst, word num);
PUBLIC void ll_cp_to_heap(void* src, addr_t dst, word num);
PUBLIC void ll_cp_heap_to_heap(addr_t src, addr_t dst, word num);
PUBLIC sreg_t ll_get_heap_seg(void);

//Heap Base on Memory Driver
PUBLIC void init_heap(void);
PUBLIC dword get_heap_used(void);
PUBLIC dword get_heap_remain(void);
PUBLIC addr_t alloc(dword sz, bool weak);
PUBLIC void free(addr_t addr);
PUBLIC byte get_heap_byte(addr_t addr);
PUBLIC word get_heap_word(addr_t addr);
PUBLIC dword get_heap_dword(addr_t addr);
PUBLIC void set_heap_byte(addr_t addr, byte val);
PUBLIC void set_heap_word(addr_t addr, word val);
PUBLIC void set_heap_dword(addr_t addr, dword val);
PUBLIC void cp_from_heap(addr_t src, void* dst, word num);
PUBLIC void cp_to_heap(void* src, addr_t dst, word num);
PUBLIC void cp_heap_to_heap(addr_t src, addr_t dst, word num);
PUBLIC sreg_t get_heap_seg(void);
PUBLIC void update_heap(void);
PUBLIC void* load_to_inner(addr_t address);
PUBLIC void save_from_inner(addr_t address);

#define LDADDR(addr) (load_to_inner(addr))
#define STADDR(addr) (save_from_inner(addr))