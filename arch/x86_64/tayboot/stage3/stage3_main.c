// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage3/stage3_main.c
 *
 * Stage3主函数
 */



#include <boot_args.h>
#include <tay_defs.h>   
#include <string.h>
#include "int_handlers.h"
#include "vedio.h"
#include "printf.h"
#include "init.h"
#include "memory.h"

void entry(struct boot_args* boot_args) {
    if (boot_args->magic != BOOT_ARGS_MAGIC) { //判断Boot Magic
        while (true);
    }
    init_gdt();
    init_pic();
    init_idt();
    init_heap(boot_args->memory_size);
    init_vedio(boot_args->screen_width, boot_args->screen_height,
    boot_args->is_graphic_mode ? DPM_GRAPHIC : DPM_CHARACTER, boot_args->framebuffer);
    //初始化GDT PIC IDT 堆 视频
    asmv ("sti"); //开中断
    if (! boot_args->is_graphic_mode) { //不是图形模式
        print_info.scroll_line = 18;
        printf ("STAGE3 Now!\n");
        printf ("Boot args Magic:%#8X!\n", boot_args->magic); //打印信息
        printf ("Memory Size: %d (B), %d (KB), %d (MB)\n", boot_args->memory_size, boot_args->memory_size / 1024, boot_args->memory_size / 1024 / 1024);
    }
}