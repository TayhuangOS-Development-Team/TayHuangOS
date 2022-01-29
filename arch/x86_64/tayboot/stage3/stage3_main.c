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

void graphic_test(int width, int height) {
    color_rgb color;

    color.r = 0xFF;
    color.g = 0xFF;
    color.b = 0xFF;

    for (int i = 0 ; i < width ; i ++) {
        for (int j = 0 ; j < 150 ; j ++) {
            draw_pixel(i, j, color);
        }
    }

    color.r = 0xFF;
    color.g = 0x00;
    color.b = 0x00;
    for (int i = 0 ; i < width ; i ++) {
        for (int j = 150 ; j < 300 ; j ++) {
            draw_pixel(i, j, color);
        }
    }

    color.r = 0x00;
    color.g = 0xFF;
    color.b = 0x00;
    for (int i = 0 ; i < width ; i ++) {
        for (int j = 300 ; j < 450 ; j ++) {
            draw_pixel(i, j, color);
        }
    }

    color.r = 0x00;
    color.g = 0x00;
    color.b = 0xFF;
    for (int i = 0 ; i < width ; i ++) {
        for (int j = 450 ; j < 600 ; j ++) {
            draw_pixel(i, j, color);
        }
    }

    color.r = 0xFF;
    color.g = 0x00;
    color.b = 0xFF;
    for (int i = 0 ; i < width ; i ++) {
        for (int j = 600 ; j < 750 ; j ++) {
            draw_pixel(i, j, color);
        }
    }

    color.r = 0x00;
    color.g = 0xFF;
    color.b = 0xFF;
    for (int i = 0 ; i < width ; i ++) {
        for (int j = 750 ; j < height ; j ++) {
            draw_pixel(i, j, color);
        }
    }
}

void int_test(void) {
    
}

void entry(struct boot_args* boot_args) {
    if (boot_args->magic != BOOT_ARGS_MAGIC) {
        while (true);
    }
    init_vedio(boot_args->screen_width, boot_args->screen_height,
    boot_args->is_graphic_mode ? DPM_GRAPHIC : DPM_CHARACTER, boot_args->framebuffer);
    if (boot_args->is_graphic_mode) {
        graphic_test(boot_args->screen_width, boot_args->screen_height);
    }
    else {
        print_info.scroll_line = 18;
        // printf ("STAGE3 Now!\n");
        // printf ("Boot args Magic:%#8X!\n", boot_args->magic);
        // printf ("Memory Size: %d (B), %d (KB), %d (MB)", boot_args->memory_size, boot_args->memory_size / 1024, boot_args->memory_size / 1024 / 1024);
    }
    int_test();
}