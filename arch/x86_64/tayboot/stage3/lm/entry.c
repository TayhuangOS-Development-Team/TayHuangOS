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
 * entry.c
 *
 * 长模式入口
 *
 */



#include "entry.h"
#include "setup_paging.h"
#include "setup_longmode.h"
#include "load_kernel.h"
#include "../printf.h"
#include "../memory.h"
#include <tayhuang/boot_args.h>
#include <tayboot/descs.h>
#include "../int_handlers.h"

PRIVATE void init_boot_args(struct boot_args *output, struct stage3_args *args, void *page_start, void *page_limit,
                            void *kernel_start, void *kernel_limit) {
    int cnt = 0;

    memset(output, 0, sizeof(struct boot_args));

    output->segments[cnt].base = 0xA0000;
    output->segments[cnt].limit = 0xFFFFF;
    output->segments[cnt ++].type = MST_RESERVED;

    output->segments[cnt].base = 0xA0000000;
    output->segments[cnt].limit = 0xFFFFFFFF;
    output->segments[cnt ++].type = MST_RESERVED;

    output->segments[cnt].base = page_start;
    output->segments[cnt].limit = page_limit;
    output->segments[cnt ++].type = MST_PAGE;

    output->segments[cnt].base = kernel_start;
    output->segments[cnt].limit = kernel_limit;
    output->segments[cnt ++].type = MST_KERNEL;

    output->segments[cnt].base = 0x00000;
    output->segments[cnt].limit = 0x9FFFF;
    output->segments[cnt ++].type = MST_FREE;

    if (min(kernel_start, page_start) > (void*)0x100000) {
        output->segments[cnt].base = 0x100000;
        output->segments[cnt].limit = min(kernel_start, page_start) - 1;
        output->segments[cnt ++].type = MST_FREE;
    }

    if (kernel_start > (page_limit + 1)) {
        output->segments[cnt].base = page_limit + 1;
        output->segments[cnt].limit = kernel_start - 1;
        output->segments[cnt ++].type = MST_FREE;
    }
    else if ((kernel_limit + 1) < page_start) {
        output->segments[cnt].base = kernel_limit + 1;
        output->segments[cnt].limit = page_start - 1;
        output->segments[cnt ++].type = MST_FREE;
    }

    if (max(kernel_limit, page_limit) < (void*)args->memory_size) {
        output->segments[cnt].base = max(kernel_limit, page_limit);
        output->segments[cnt].limit = args->memory_size - 1;
        output->segments[cnt ++].type = MST_FREE;
    }

    output->magic = BOOT_ARGS_MAGIC;
    output->is_graphic_mode = args->is_graphic_mode;
    output->screen_width = args->screen_width;
    output->screen_height = args->screen_height;
    output->framebuffer = args->framebuffer;
    output->memory_size = args->memory_size;
    output->memory_size_high = args->memory_size_high;
}

PUBLIC void goto_ia32e(void *entrypoint, void *args, word selector64);

PUBLIC void goto_longmode(struct stage3_args *args, word selector64) {
    void *kernel_start, *kernel_limit;
    void *entry_point = load_kernel(&kernel_start, &kernel_limit);
    void *page_limit;
    void *page_start = setup_paging(args->memory_size, args->memory_size_high, &page_limit); //设置分页
    setup_longmode(page_start); //设置长模式
    printf ("Long mode already!\n");
    static struct boot_args _args;
    init_boot_args(&_args, args, page_start, page_limit, kernel_start, kernel_limit);
    for (int i = 0 ; i < 16 ; i ++)
        disable_irq(i);
    asmv ("cli");
    static struct desc_ptr _idtr = {.address = 0, .size = 0};
    asmv ("lidt %0" : : "m"(_idtr));
    goto_ia32e(entry_point, &_args, selector64);
}