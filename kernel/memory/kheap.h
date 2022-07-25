/*
 * SPDX-License-Identifier: LGPL-2.1-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: theflysong
 *
 * kheap.h
 *
 * 内核堆
 *
 */



#pragma once

#include <tayhuang/defs.h>

PUBLIC void init_kheap(void *prepare_base, size_t prepare_size);
PUBLIC void *kmalloc(size_t size); //分配内存
static inline void *kcalloc(int num, size_t size) { //分配num个size的内存
    return kmalloc(num * size);
}
PUBLIC void kfree(void *ptr); //释放内存