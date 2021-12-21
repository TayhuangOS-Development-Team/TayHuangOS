// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/heap.h
 *
 * Heap manage function is declared here
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
PUBLIC void ll_cp_to(addr_t src, addr_t dst, word num);
PUBLIC void ll_cp_from(addr_t src, addr_t dst, word num);