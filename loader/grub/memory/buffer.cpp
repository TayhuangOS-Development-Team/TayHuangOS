/**
 * @file buffer.cpp
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 缓存
 * @version alpha-1.0.0
 * @date 2022-12-14
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <memory/buffer.h>
#include <tayhuang/types.h>
#include <cstdbool>
#include <cstring>

PRIVATE byte _BUFFER[BUFFER_NUM * BUFFER_SIZE];
PRIVATE bool _FLAGS[BUFFER_NUM];

PUBLIC void buffer_init(void) {
    memset(_FLAGS, 0, sizeof(_FLAGS));
}

PUBLIC void *alloc_buffer(void) {
    int i;
    for (i = 0 ; i < BUFFER_NUM ; i ++) {
        if (! _FLAGS[i]) {
            break;
        }
    }
    if (i >= BUFFER_NUM) {
        return NULL;
    }
    _FLAGS[i] = true;
    return &_BUFFER[0] + i * BUFFER_SIZE;
}

PUBLIC void free_buffer(void *buffer) {
    byte *_buffer = (byte *)buffer;
    int idx = (_buffer - &_BUFFER[0]) / BUFFER_SIZE;
    if (idx < 0 || idx >= BUFFER_NUM) {
        return;
    }
    _FLAGS[idx] = false;
}