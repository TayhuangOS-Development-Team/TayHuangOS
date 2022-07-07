/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: theflysong
 *
 * common.c
 *
 * 通用FS接口函数
 *
 */


#include <fs/common.h>
#include <fs/fat32.h>

#include <tayhuang/partition.h>
#include <disk.h>

#include <lheap.h>

#include <printf.h>

#include <ctype.h>
#include <string.h>


typedef enum {
    FS_UNKNOWN = 0,
    FS_FAT32,
    FS_NTFS,
    FS_EXT2
} FS_TYPES;

PRIVATE FS_TYPES get_fs_type(int disk_selector, int partition_id) {
    partition_member partition;
    get_partition(disk_selector, partition_id, &partition); //获取分区

    void *superblock = lmalloc(512);
    read_sector(partition.start_lba, 1,  disk_selector, superblock); //读取超级块

    char bpb_filesystem[9];

    memcpy(bpb_filesystem, superblock + 0x36, 8); //
    bpb_filesystem[8] = '\0';

    lfree (superblock);

    if (strcmp(bpb_filesystem, "fat32")) { //是fat32
        return FS_FAT32;
    }

    return FS_UNKNOWN;
}

PUBLIC fs_context load_fs(int disk_selector, int partition_id){
    FS_TYPES type = get_fs_type(disk_selector, partition_id); //获取类型

    if (type == FS_FAT32) {
        return load_fat32_fs(disk_selector, partition_id);
    }
    else {
        return NULL;
    }
}

PUBLIC void display_fs_info(fs_context context) {
    if (*((dword*)context) == FAT32_CONTEXT_MAGIC) { //比较context魔数
        display_fat32_fs_info(context);
    }
}

PUBLIC bool load_file(fs_context context, const char *name, void *dst) {
    if (*((dword*)context) == FAT32_CONTEXT_MAGIC) { //比较context魔数
        return load_fat32_file(context, name, dst);
    }
    return false;
}

PUBLIC void terminate_fs(fs_context context) {
    if (*((dword*)context) == FAT32_CONTEXT_MAGIC) { //比较context魔数
        terminate_fat32_fs(context);
    }
}