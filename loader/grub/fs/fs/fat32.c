/**
 * @file fat32.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief FAT32
 * @version alpha-1.0.0
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <fs/fs/fat32.h>
#include <string.h>
#include <logging.h>

#define BPB_FILESYSTEM (0x52)

PUBLIC bool is_fat32_fs(void *pbr) {
    char fs_type[9];
    memcpy(fs_type, pbr + BPB_FILESYSTEM, 8);
    fs_type[8] = '\0';
    return strcmp(fs_type, "FAT32   ") == 0;
}

PUBLIC fs_t *open_fat32_fs(void *pbr) {
    return NULL;
}

PUBLIC size_t get_fat32_fs_size(void) {
    return 0;
}