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
 * mm_malloc.h
 * 
 * malloc系函数
 * 
 */



#pragma once

#include <tayhuang/defs.h>

PUBLIC void *cpmalloc(int size); //size向上取页大小的倍数 连续物理空间
PUBLIC void *tmalloc(int size, int pid); //size向上取页大小的倍数
PUBLIC void *vmalloc(int size);
static inline void *cpcalloc(int size, int num) {
    return cpmalloc(size * num);
}
static inline void *tcalloc(int size, int num, int pid) {
    return tmalloc(size * num, pid);
}
static inline void *vcalloc(int size, int num) {
    return vmalloc(size * num);
}
PUBLIC void cpfree(void *addr, int size);
PUBLIC void tfree(void *addr, int size, int pid);
PUBLIC void vfree(void *addr, int size);