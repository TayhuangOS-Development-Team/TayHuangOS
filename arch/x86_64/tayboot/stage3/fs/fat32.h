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
 * arch/x86_64/tayboot/stage3/fs/fat32.h
 *
 * FAT32文件系统
 */



#pragma once

#include <tayboot/tay_defs.h>

PUBLIC void *get_context(int selector);
PUBLIC void print_fs_info(void *context);
PUBLIC bool loadfile(void *context, const char *name, void *dst);
PUBLIC void terminate_fs_context(void *context);