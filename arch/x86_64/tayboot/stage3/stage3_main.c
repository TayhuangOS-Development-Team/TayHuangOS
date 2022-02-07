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
 * stage3_main.c
 *
 * Stage3主函数
 *
 */



#include <tayboot/stage3_args.h>
#include <tayboot/tay_defs.h>   
#include <string.h>
#include "int_handlers.h"
#include "video.h"
#include "printf.h"
#include "init.h"
#include "memory.h"
#include "lm/entry.h"
#include "disk.h"

void print_star(int irq) {
    putchar('*');
}

void entry(struct stage3_args* _stage3_args) {
    if (_stage3_args->magic != STAGE3_ARGS_MAGIC) { //判断Stage3 Magic
        while (true);
    }
    struct stage3_args stage3_args;
    memcpy(&stage3_args, _stage3_args, sizeof(struct stage3_args)); //参数

    asmv ("finit"); //初始化FPU

    init_gdt();
    init_pic();
    init_idt();
    init_heap(stage3_args.memory_size);
    init_video(stage3_args.screen_width, stage3_args.screen_height,
    stage3_args.is_graphic_mode ? DPM_GRAPHIC : DPM_CHARACTER, stage3_args.framebuffer);
    print_info.scroll_line = 18;
    //初始化GDT PIC IDT 堆 视频

    asmv ("sti"); //开中断
    enable_irq(2); //开启IRQ2(桥接)
    init_disk_driver(); //初始化硬盘驱动

    goto_longmode(&stage3_args, 4 << 3); //进入长模式
}