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
    memset(output, 0, sizeof(struct boot_args));

    output->magic = BOOT_ARGS_MAGIC; //魔数

    output->is_graphic_mode = args->is_graphic_mode;
    output->screen_width = args->screen_width;
    output->screen_height = args->screen_height;
    output->framebuffer = args->framebuffer; //视频
    output->framebuffer &= 0xFFFFFFFF;

    output->memory_size = args->memory_size;
    output->memory_size_high = args->memory_size_high; //内存

    output->kernel_start = kernel_start;
    output->kernel_limit = kernel_limit;
    output->page_start = page_start;
    output->page_limit = page_limit; //内存段

    output->setup_mod_addr = SETUP_MOD_ADDRESS;
}

PUBLIC void goto_ia32e(void *entrypoint, void *args, word selector64);

PUBLIC void goto_longmode(struct stage3_args *args, word selector64) {
    void *kernel_start, *kernel_limit;
    void *entry_point = load_kernel(&kernel_start, &kernel_limit); //加载内核

    void *page_limit;
    void *page_start = setup_paging(args->memory_size, args->memory_size_high, &page_limit); //设置分页

    setup_longmode(page_start); //设置长模式

    static struct boot_args _args;
    init_boot_args(&_args, args, page_start, page_limit, kernel_start, kernel_limit); //初始化引导参数

    for (int i = 0 ; i < 16 ; i ++)
        disable_irq(i); //禁用所有IRQ
    asmv ("cli"); //关中断
    static struct desc_ptr _idtr = {.address = 0, .size = 0};
    asmv ("lidt %0" : : "m"(_idtr)); //重置IDTR

    goto_ia32e(entry_point, &_args, selector64); //进入IA32-e模式
}