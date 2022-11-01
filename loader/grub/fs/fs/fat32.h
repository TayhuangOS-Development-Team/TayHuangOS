/**
 * @file fat32.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief FAT32
 * @version alpha-1.0.0
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#pragma once

#include <fs/vfs.h>

PUBLIC bool is_fat32_fs(void *pbr);
PUBLIC fs_t *open_fat32_fs(void *pbr);
PUBLIC size_t get_fat32_fs_size(void);