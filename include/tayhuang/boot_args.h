/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: theflysong
 *
 * boot_args.h
 *
 * boot_args的结构
 *
 */



#pragma once

struct boot_args {
    unsigned int magic; //魔数 用于校验
    unsigned char is_graphic_mode; //是否为图形模式
    int screen_width; //屏幕宽
    int screen_height; //屏幕高
    unsigned long long framebuffer; //屏幕显存
    unsigned int memory_size; //内存大小
    unsigned int memory_size_high; //内存大小(大)
    unsigned long long kernel_start;
    unsigned long long kernel_limit;
    unsigned long long page_start;
    unsigned long long page_limit;
    unsigned long long setup_mod_addr;
};

#define BOOT_ARGS_MAGIC (0x5A71F213) //MD5 of "BOOTARGS"(后32位)