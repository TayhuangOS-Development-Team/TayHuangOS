/**
 * @file heap.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief HEAP
 * @version alpha-1.0.0
 * @date 2022-10-22
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#pragma once

#include <tayhuang/types.h>
#include <tayhuang/attributes.h>

__C_HEADER_START;

EXPORT void *malloc(size_t size);
EXPORT void free(void *p);

__C_HEADER_END;