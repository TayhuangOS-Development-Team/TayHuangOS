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
 * common.h
 *
 * 通用FS接口函数
 *
 */



#pragma once

#include <tayhuang/defs.h>

typedef void *fs_context;

PUBLIC fs_context load_fs(int disk_selector, int partition_id);
PUBLIC bool load_file(fs_context context, const char *name, void *dst, bool show_progress);
PUBLIC void terminate_fs(fs_context context);
PUBLIC void display_fs_info(fs_context context);