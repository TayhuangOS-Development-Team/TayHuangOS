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

#include <tayhuang/types.h>

void get_context(int selector, void **__context);
void print_fs_info(void *context);
bool loadfile(void *context, const char *name, void *dst);