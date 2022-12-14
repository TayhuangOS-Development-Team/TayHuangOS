/**
 * @file cpp_abi.cpp
 * @author theflysong (song_of_the_fly@163.com)
 * @brief ABI
 * @version alpha-1.0.0
 * @date 2022-12-14
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <heap.h>

IMPL void *operator new(size_t size) {
    return malloc(size);
}

IMPL void *operator new[](size_t size) {
    return malloc(size);
}

IMPL void operator delete(void *p) {
    free(p);
}

IMPL void operator delete[](void *p) {
    free(p);
}