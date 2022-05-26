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
 * memory.h
 *
 * 内存管理函数
 *
 */



#pragma once

#include <tayboot/tay_defs.h>

PUBLIC void memcpy(void *dst, void *src, int sz); //复制内存
PUBLIC void memset(void *dst, byte val, int sz); //设置内存
PUBLIC void init_heap(int memsz); //初始化堆
PUBLIC void terminate_heap(int memsz); //析构堆
PUBLIC void reset_heap(int memsz); //重置堆
PUBLIC void *malloc(int size); //分配内存
PUBLIC void *calloc(int num, int size); //分配num个size的内存
PUBLIC void free(void *ptr); //释放内存
PUBLIC bool query_using(void *ptr, int size); //这一段是否有被使用 true when using
PUBLIC void set_heap_top(void *top); //设置堆顶 default = 0xFFFF
PUBLIC void set_heap_limit(void *limit); //设置堆底 default = 0xC000
PUBLIC int get_used(void); //有多少被使用
PUBLIC int get_remain(void); //有多少没被使用