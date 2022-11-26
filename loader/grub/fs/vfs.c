/**
 * @file vfs.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief VFS
 * @version alpha-1.0.0
 * @date 2022-10-30
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <fs/vfs.h>

#include <heap.h>
#include <logging.h>

#include <string.h>

/**
 * @brief 操作数
 * 
 */
#define NUM_SOP (16)
/**
 * @brief 操作列表
 * 
 */
PRIVATE superop_t OPERATORS[NUM_SOP] = {};