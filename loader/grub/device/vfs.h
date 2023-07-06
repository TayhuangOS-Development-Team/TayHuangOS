/**
 * @file vfs.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief VFS
 * @version alpha-1.0.0
 * @date 2023-06-25
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#pragma once

#include <device/disk.h>

/**
 * @brief 文件系统操作
 * 
 */
typedef struct {
    /** 识别文件系统 */
    bool (*identity)(disk_t *disk, partition_t *part, void *bootsect);
    /** 加载文件系统 */
    void *(*load)(disk_t *disk, partition_t *part);
    /** 读文件 */
    bool  (*rdfile)(disk_t *disk, partition_t *part, void *fs, const char *name, void *dst);
    /** 卸载文件系统 */
    void (*unload)(void *fs);
} fsops_t;

/**
 * @brief 识别文件系统
 * 
 * @param disk 硬盘
 * @param part 分区
 * @return NULL 识别失败
 * @return FS操作表
 */
const fsops_t *idenfs(disk_t *disk, partition_t *part);