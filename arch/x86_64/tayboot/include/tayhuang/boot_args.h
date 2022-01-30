// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/include/tayhuang/boot_args.h
 *
 * boot_args的结构
 */



#pragma once

#include <tay_type.h>

struct boot_args {
    dword magic;
    bool is_graphic_mode;
    int screen_width;
    int screen_height;
    void* framebuffer;
    int memory_size;
};

#define BOOT_ARGS_MAGIC (0x553DBB2) //MD5 of "BOOTARGS"(前32位)