/**
 * @file vfs.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief VFS
 * @version alpha-1.0.0
 * @date 2023-06-25
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <bcl/logger.h>
#include <device/vfs.h>
#include <stddef.h>

/** FSOP表大小 */
#define FSOP_SIZE (32)

/** FSOP表 */
const fsops_t FSOP[FSOP_SIZE] = {
    {.identity = NULL, .load = NULL, .rdfile = NULL, .unload = NULL}
};

/**
 * @brief 识别文件系统
 * 
 * @param disk 硬盘
 * @param part 分区
 * @return NULL 识别失败
 * @return FS操作表
 */
const fsops_t *idenfs(disk_t *disk, partition_t *part) {
    char bootsect[512];
    read_part_sector(disk, part, 0, 1, bootsect);
    
    // 遍历表
    for (int i = 0 ; i < FSOP_SIZE ; i ++) {
        if (FSOP[i].identity != NULL) {
            if (FSOP[i].identity(disk, part, bootsect)) {
                return &FSOP[i];
            }
        }
    }

    lerror("Unknown FS!");
    return NULL;
}