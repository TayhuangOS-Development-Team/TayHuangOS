/**
 * @file heap.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 堆
 * @version alpha-1.0.0
 * @date 2022-12-31
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <heap.h>
#include <tay/types.h>

static byte __HEAP__[HEAP_SIZE];
static byte *__heap_ptr__ = __HEAP__;

void *malloc(size_t size) {
    void *ret = __heap_ptr__;
    __heap_ptr__ += size;
    return ret;
}

void free(void *ptr) {
    // 没想到吧我摆烂了嘿嘿
}