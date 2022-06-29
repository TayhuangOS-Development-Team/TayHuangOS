/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * kheap.h
 *
 * 内核堆
 *
 */



#pragma once

#include <tayhuang/defs.h>

#define KHEAP_BASE (0x100000)

PUBLIC void init_kheap(void *kheap_limit); //初始化堆
PUBLIC void *kmalloc(int size); //分配内存
static inline void *kcalloc(int num, int size) { //分配num个size的内存
    return kmalloc(num * size);
}
PUBLIC void kfree(void *ptr); //释放内存