// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage2/buffer.h
 *
 * Buffer manage functions are declared here
 */



#pragma once

#include "header.h"

//Low level buffer
PUBLIC void ll_init_buffer(void);
PUBLIC void ll_resize_buffer(word sz);
PUBLIC word ll_get_buffer_used(void);
PUBLIC word ll_get_buffer_remain(void);
PUBLIC addr_t ll_alloc_buffer(word sz);
PUBLIC void ll_free_buffer(addr_t addr);
PUBLIC byte ll_get_byte_buffer(addr_t addr);
PUBLIC word ll_get_word_buffer(addr_t addr);
PUBLIC dword ll_get_dword_buffer(addr_t addr);
PUBLIC void ll_set_byte_buffer(addr_t addr, byte val);
PUBLIC void ll_set_word_buffer(addr_t addr, word val);
PUBLIC void ll_set_dword_buffer(addr_t addr, dword val);
PUBLIC void ll_cp_from_buffer(addr_t src, void* dst, word num);
PUBLIC void ll_cp_to_buffer(void* src, addr_t dst, word num);
PUBLIC void ll_cp_buffer_to_buffer(addr_t src, addr_t dst, word num);
PUBLIC sreg_t ll_get_buffer_seg(void);

//Buffer Base on Memory Driver
PUBLIC void init_buffer(void);
PUBLIC dword get_buffer_used(void);
PUBLIC dword get_buffer_remain(void);
PUBLIC addr_t alloc_buffer(dword sz, bool weak);
PUBLIC void free_buffer(addr_t addr);
PUBLIC byte get_buffer_byte(addr_t addr);
PUBLIC word get_buffer_word(addr_t addr);
PUBLIC dword get_buffer_dword(addr_t addr);
PUBLIC void set_buffer_byte(addr_t addr, byte val);
PUBLIC void set_buffer_word(addr_t addr, word val);
PUBLIC void set_buffer_dword(addr_t addr, dword val);
PUBLIC void cp_from_buffer(addr_t src, void* dst, word num);
PUBLIC void cp_to_buffer(void* src, addr_t dst, word num);
PUBLIC void cp_buffer_to_buffer(addr_t src, addr_t dst, word num);
PUBLIC sreg_t get_buffer_seg(void);
PUBLIC void update_buffer(void);