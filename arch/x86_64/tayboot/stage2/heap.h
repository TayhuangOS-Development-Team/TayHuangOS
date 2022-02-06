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
 * heap.h
 *
 * 堆管理函数
 *
 */



#pragma once

#include "header.h"

PUBLIC void init_heap(void); //初始化堆
PUBLIC void terminate_heap(void); //释放堆
PUBLIC void reset_heap(void); //重置堆
PUBLIC void *malloc(int size); //分配内存
PUBLIC void *calloc(int num, int size); //分配内存
PUBLIC void free(void *ptr); //释放内存
PUBLIC bool query_using(void *ptr, int size); //查询某段内存是否有被使用, 使用时为真
PUBLIC void set_heap_top(void *top); //默认值为0xFFFF
PUBLIC void set_heap_limit(void *limit); //默认值为0xC000
PUBLIC int get_used(void); //已用的
PUBLIC int get_remain(void); //剩余的