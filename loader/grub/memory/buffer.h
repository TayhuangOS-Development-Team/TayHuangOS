/**
 * @file buffer.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 缓存
 * @version alpha-1.0.0
 * @date 2022-12-14
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#pragma once

#include <tayhuang/attributes.h>

#define BUFFER_SIZE (1024)
#define BUFFER_NUM (512)

PUBLIC void buffer_init(void);
PUBLIC void *alloc_buffer(void);
PUBLIC void free_buffer(void *buffer);