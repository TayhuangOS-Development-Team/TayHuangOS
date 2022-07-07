/*
 * SPDX-License-Identifier: GPL-3.0-only
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

PUBLIC void init_kheap(void *prepare_base, int prepare_size);
PUBLIC void *kmalloc(int size); //分配内存
static inline void *kcalloc(int num, int size) { //分配num个size的内存
    return kmalloc(num * size);
}
PUBLIC void kfree(void *ptr); //释放内存