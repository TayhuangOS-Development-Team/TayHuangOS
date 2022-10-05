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
 * common.h
 *
 * 通用FS接口函数
 *
 */



#pragma once

#include <tayhuang/defs.h>

//文件系统上下文
typedef void *fs_context;

//加载文件系统上下文
PUBLIC fs_context load_fs(int disk_selector, int partition_id);
//读取文件
PUBLIC bool load_file(fs_context context, NONNULL const char *name, NONNULL void *dst);
//释放文件系统上下文
PUBLIC void terminate_fs(fs_context context);
//显示文件系统信息
PUBLIC void display_fs_info(fs_context context);