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
    dword magic; //魔数 用于校验
    bool is_graphic_mode; //是否为图形模式
    int screen_width; //屏幕宽
    int screen_height; //屏幕高
    void* framebuffer; //屏幕显存
    int memory_size; //内存大小
};

#define BOOT_ARGS_MAGIC (0x553DBB2) //MD5 of "BOOTARGS"(前32位)