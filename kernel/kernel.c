/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 * 
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 * 
 * --------------------------------------------------------------------------------
 * 
 * 作者: Flysong
 * 
 * kernel.c
 * 
 * 内核主程序
 *
 */



#include <tayhuang/boot_args.h>
#include <tayhuang/defs.h>
#include "memory/segment.h"
#include "kheap.h"
#include "display/video.h"
#include "display/printk.h"

void entry(struct boot_args *_args) {
    if (_args->magic != BOOT_ARGS_MAGIC) {
        while (true);
    }
    struct boot_args args;
    memcpy(&args, _args, sizeof(struct boot_args));

    init_video(args.framebuffer, args.screen_width, args.screen_height, args.is_graphic_mode);

    set_print_color(0x0F);
    set_scroll_line(18);
    puts ("Hello, World0!\n");
    puts ("Hello, World1!\n");
    puts ("Hello, World2!\n");
    puts ("Hello, World3!\n");
    puts ("Hello, World4!\n");
    puts ("Hello, World5!\n");
    puts ("Hello, World6!\n");
    puts ("Hello, World7!\n");
    
    scroll_screen(-1);

    init_gdt();
    init_kheap();
}