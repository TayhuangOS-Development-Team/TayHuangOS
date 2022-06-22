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

PUBLIC fs_context load_fs(int partition_id, int disk_selector);
PUBLIC void load_file(fs_context context, const char *name, void *dst);
PUBLIC void terminate_fs(fs_context context);