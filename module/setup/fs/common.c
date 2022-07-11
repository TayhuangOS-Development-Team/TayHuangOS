/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * common.c
 *
 * 通用FS接口函数
 *
 */


#include <fs/common.h>
#include <fs/fat32.h>
#include <disk.h>
#include <printf.h>
#include <ctype.h>
#include <string.h>
#include <tayhuang/partition.h>
#include <memory/malloc.h>

typedef enum {
    FS_UNKNOWN = 0,
    FS_FAT32,
    FS_NTFS,
    FS_EXT2
} FS_TYPES;

PRIVATE FS_TYPES get_fs_type(int disk_selector, int partition_id) {
    partition_info partition;
    get_partition(disk_selector, partition_id, &partition); //获取分区

    void *superblock = malloc(512);
    read_sector(partition.start_lba, 1,  disk_selector, superblock); //读取超级块

    char bpb_filesystem[9];

    memcpy(bpb_filesystem, superblock + 0x36, 8);
    bpb_filesystem[8] = '\0';

    free (superblock);

    if (strcmp(bpb_filesystem, "fat32")) {
        return FS_FAT32;
    }

    return FS_UNKNOWN;
}

PUBLIC fs_context load_fs(int disk_selector, int partition_id){
    FS_TYPES type = get_fs_type(disk_selector, partition_id);
    if (type == FS_FAT32) {
        return load_fat32_fs(disk_selector, partition_id);
    }
    else {
        return NULL;
    }
}

PUBLIC void display_fs_info(fs_context context) {
    if (*((dword*)context) == FAT32_CONTEXT_MAGIC) {
        display_fat32_fs_info(context);
    }
}

PUBLIC bool load_file(fs_context context, const char *name, void *dst, bool show_progress) {
    if (*((dword*)context) == FAT32_CONTEXT_MAGIC) {
        return load_fat32_file(context, name, dst, show_progress);
    }
    return false;
}

PUBLIC void terminate_fs(fs_context context) {
    if (*((dword*)context) == FAT32_CONTEXT_MAGIC) {
        terminate_fat32_fs(context);
    }
}