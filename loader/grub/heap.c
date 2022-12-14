/**
 * @file heap.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief HEAP
 * @version alpha-1.0.0
 * @date 2022-10-22
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <heap.h>

#define HEAP_SIZE (512 * 1024) //512 KB

PRIVATE byte _HEAP[HEAP_SIZE];
PRIVATE byte *_heap_ptr = _HEAP;

EXPORT void *malloc(size_t size) {
    void *ret = _heap_ptr;
    _heap_ptr += (size + 0x7) & ~0x7;
    return ret;
}

EXPORT void free(void *p) {
}