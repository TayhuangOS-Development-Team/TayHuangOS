/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage2/buffer.h
 *
 * 缓存管理函数
 *
 */



#pragma once

#include "header.h"

//低级缓存
PUBLIC void ll_init_buffer(void); //初始化
PUBLIC void ll_resize_buffer(word sz); //重置大小
PUBLIC word ll_get_buffer_used(void); //用了多少
PUBLIC word ll_get_buffer_remain(void); //还剩多少
PUBLIC addr_t ll_alloc_buffer(word sz); //分配一块缓存
PUBLIC void ll_free_buffer(addr_t addr); //释放一块缓存
PUBLIC byte ll_get_byte_buffer(addr_t addr); //取字节
PUBLIC word ll_get_word_buffer(addr_t addr); //取字
PUBLIC dword ll_get_dword_buffer(addr_t addr); //取双字
PUBLIC void ll_set_byte_buffer(addr_t addr, byte val); //设字节
PUBLIC void ll_set_word_buffer(addr_t addr, word val); //设字
PUBLIC void ll_set_dword_buffer(addr_t addr, dword val); //设双字
PUBLIC void ll_cp_from_buffer(addr_t src, void *dst, word num); //从缓存中复制
PUBLIC void ll_cp_to_buffer(void *src, addr_t dst, word num); //复制到缓存
PUBLIC void ll_cp_buffer_to_buffer(addr_t src, addr_t dst, word num); //从缓存复制到缓存
PUBLIC sreg_t ll_get_buffer_seg(void); //获取缓存段

//基于内存驱动的缓存
PUBLIC void init_buffer(void); //初始化
PUBLIC dword get_buffer_used(void); //用了多少
PUBLIC dword get_buffer_remain(void); //还剩多少
PUBLIC addr_t alloc_buffer(dword sz, bool weak); //分配一块缓存
PUBLIC void free_buffer(addr_t addr); //释放一块缓存
PUBLIC byte get_buffer_byte(addr_t addr); //取字节
PUBLIC word get_buffer_word(addr_t addr); //取字
PUBLIC dword get_buffer_dword(addr_t addr); //取双字
PUBLIC void set_buffer_byte(addr_t addr, byte val); //设字节
PUBLIC void set_buffer_word(addr_t addr, word val); //设字
PUBLIC void set_buffer_dword(addr_t addr, dword val); //设双字
PUBLIC void cp_from_buffer(addr_t src, void *dst, word num); //从缓存中复制
PUBLIC void cp_to_buffer(void *src, addr_t dst, word num); //复制到缓存
PUBLIC void cp_buffer_to_buffer(addr_t src, addr_t dst, word num); //从缓存复制到缓存
PUBLIC sreg_t get_buffer_seg(void); //获取缓存段
PUBLIC void update_buffer(void); //更新 没啥用 暂时先不必管它