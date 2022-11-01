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

/**
 * @brief 是否为fat32文件系统
 * 
 * @param pbr 分区引导记录
 * @return true 是
 * @return false 不是
 */
PUBLIC bool is_fat32_fs(void *pbr);
/**
 * @brief 打开fat32文件系统
 * 
 * @param pbr 分区引导记录
 * @return fat32文件系统
 */
PUBLIC fs_t *open_fat32_fs(void *pbr);
/**
 * @brief 关闭fat32文件系统
 * 
 * @param fs fat32文件系统
 */
PUBLIC void close_fat32_fs(fs_t *fs);