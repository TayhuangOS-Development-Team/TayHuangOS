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

void entry(_IN struct boot_args *_args) {
    if (_args->magic != BOOT_ARGS_MAGIC) {
        while (true);
    }
    struct boot_args args;
    memcpy(&args, _args, sizeof(struct boot_args));

    init_video(args.framebuffer, args.screen_width, args.screen_height, args.is_graphic_mode);

    set_print_color(0x0F);
    set_scroll_line(18);
    printk ("Hello, World!%d\n", 114514);

    init_kheap();
    init_gdt();

    SEGMENT_TOKEN KERNEL_TOKEN, KHEAP_TOKEN;
    init_segments(args.kernel_start, args.kernel_limit, &KERNEL_TOKEN, &KHEAP_TOKEN);
}