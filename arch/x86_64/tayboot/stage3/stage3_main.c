// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage3/stage3_main.c
 *
 * Stage3 main function here
 */



#include <boot_args.h>
#include <tay_defs.h>   
#include <string.h>
#include "vedio.h"
#include "printf.h"
#include "init.h"
#include "memory.h"

void graphic_test(int width, int height) {
    color_rgb color;
    color.r = 0xFF;
    color.g = 0xFF;
    color.b = 0xFF;
    draw_rectangle(0, 0, width, height, color);

    color.r = 0x40;
    color.g = 0x40;
    color.b = 0x40;
    //山
    //三竖
    draw_rectangle(60, 60, 100, 270, color);
    draw_rectangle(170, 20, 210, 270, color);
    draw_rectangle(280, 60, 320, 270, color);
    //一横
    draw_rectangle(60, 270, 320, 310, color);

    //本
    //两横
    draw_rectangle(440, 60, 780, 100, color);
    draw_rectangle(520, 230, 700, 270, color);
    //一竖
    draw_rectangle(590, 20, 630, 310, color);
    //一撇
    for (int i = 0 ; i < 40 ; i ++) {
        draw_rectangle(550 - i * 3, 100 + i * 3, 590 - i * 3, 140 + i * 3, color);
    }
    //一捺
    for (int i = 0 ; i <40 ; i ++) {
        draw_rectangle(630 + i * 3, 100 + i * 3, 670 + i * 3, 140 + i * 3, color);
    }
}

void entry(struct boot_args* boot_args) {
    if (boot_args->magic != BOOT_ARGS_MAGIC) {
        while (true);
    }
    init_gdt();
    init_8259A();
    init_idt();
    init_heap(boot_args->memory_size);
    init_vedio(boot_args->screen_width, boot_args->screen_height,
    boot_args->is_graphic_mode ? DPM_GRAPHIC : DPM_CHARACTER, boot_args->framebuffer);
    asmv ("sti");
    if (boot_args->is_graphic_mode) {
        graphic_test(boot_args->screen_width, boot_args->screen_height);
    }
    else {
        print_info.scroll_line = 18;
        printf ("STAGE3 Now!\n");
        printf ("Boot args Magic:%#8X!\n", boot_args->magic);
        printf ("Memory Size: %d (B), %d (KB), %d (MB)", boot_args->memory_size, boot_args->memory_size / 1024, boot_args->memory_size / 1024 / 1024);
    }
}