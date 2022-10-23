/**
 * @file heap.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 堆
 * @version alpha-1.0.0
 * @date 2022-10-23
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#pragma once

#include <tayhuang/attributes.h>
#include <tayhuang/types.h>

/**
 * @brief 堆大小(512KB)
 * 
 */
#define HEAP_SIZE (524288)

/**
 * @brief 分配内存
 * 
 * @param size 要求大小
 * @return 分配的内存地址 NULL表示没有合适内存
 */
NULLABLE void *malloc(int size);
/**
 * @brief 释放内存
 * 
 * 释放未分配的内存时会导致未知情况
 * 
 * @param addr 要释放的内存地址
 */
void free(NONNULL void *addr);