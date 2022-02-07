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

void entry(struct boot_args *_args) {
    if (_args->magic != BOOT_ARGS_MAGIC) {
        while (true);
    }
    struct boot_args args;
    memcpy(&args, _args, sizeof(struct boot_args));

    init_video(args.framebuffer, args.screen_width, args.screen_height, args.is_graphic_mode);

    init_gdt();
    init_kheap();
}