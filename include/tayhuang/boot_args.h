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
 * boot_args.h
 *
 * boot_args的结构
 *
 */



#pragma once

struct boot_args {
    int magic; //魔数 用于校验
    unsigned char is_graphic_mode; //是否为图形模式
    int screen_width; //屏幕宽
    int screen_height; //屏幕高
    long long framebuffer; //屏幕显存
    int memory_size; //内存大小
    int memory_size_high; //内存大小(大)
    long long kernel_start;
    long long kernel_limit;
    long long page_start;
    long long page_limit;
};

#define BOOT_ARGS_MAGIC (0x5A71F213) //MD5 of "BOOTARGS"(前32位)