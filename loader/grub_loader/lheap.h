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
 * lheap.h
 *
 * Loader堆
 *
 */



#pragma once

#include <tayhuang/defs.h>

#define LHEAP_BASE (0x2200000)

PUBLIC void init_lheap(NONNULL void *lheap_size); //初始化堆
PUBLIC NULLABLE("Have no free memory") void *lmalloc(int size); //分配内存
INLINE NULLABLE("Have no free memory") void *lcalloc(int num, int size) { //分配num个size的内存
    return lmalloc(num * size);
}
PUBLIC void lfree(NONNULL void *ptr); //释放内存