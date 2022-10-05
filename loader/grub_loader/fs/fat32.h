/* 
 * SPDX-License-Identifier: LGPL-2.1-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: theflysong
 *
 * fat32.h
 *
 * FAT32文件系统
 *
 */



#pragma once

#include <tayhuang/defs.h>
#include <fs/common.h>

#define FAT32_CONTEXT_MAGIC (0x93186A8E)
PUBLIC fs_context load_fat32_fs(int disk_selector, int partition_id);
PUBLIC bool load_fat32_file(fs_context context, NONNULL const char *name, NONNULL void *dst);
PUBLIC void terminate_fat32_fs(fs_context context);
PUBLIC void display_fat32_fs_info(fs_context context);