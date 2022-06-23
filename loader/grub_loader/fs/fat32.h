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
 * fat32.h
 *
 * FAT32文件系统
 *
 */



#pragma once

#include <tayhuang/defs.h>
#include "common.h"

PUBLIC fs_context load_fat32_fs(int disk_selector, int partition_id);
PUBLIC bool load_fat32_file(fs_context context, const char *name, void *dst, bool show_progress);
PUBLIC void terminate_fat32_fs(fs_context context);
PUBLIC void display_fat32_fs_info(fs_context context);