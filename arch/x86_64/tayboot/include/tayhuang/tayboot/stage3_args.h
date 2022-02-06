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
 * stage3_args.h
 *
 * boot_args的结构
 *
 */



#pragma once

#include <tayboot/tay_type.h>

struct stage3_args {
    dword magic; //魔数 用于校验
    bool is_graphic_mode; //是否为图形模式
    int screen_width; //屏幕宽
    int screen_height; //屏幕高
    void *framebuffer; //屏幕显存
    dword memory_size; //内存大小
    dword memory_size_high; //内存大小(大)
};

#define STAGE3_ARGS_MAGIC (0xD5A1B8A0) //MD5 of "STAGE3ARGS"(前32位)