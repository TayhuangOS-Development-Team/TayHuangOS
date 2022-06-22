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

PUBLIC fs_context load_fs(int partition_id, int disk_selector);
PUBLIC void load_file(fs_context context, const char *name);
PUBLIC void terminate_fs(fs_context context);