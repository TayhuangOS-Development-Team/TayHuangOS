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
#include <tayhuang/paging.h>

#include "kheap.h"

#include "memory/segment.h"
#include "memory/paging.h"

#include "display/video.h"
#include "display/printk.h"

#include "intterup/init_int.h"
#include "intterup/irq_handler.h"

#include "init/init_clock.h"
#include "init/init_misc.h"

qword init_video_info(_IN struct boot_args *args, _IN qword mapping_start) {
    int buffersz = (args->is_graphic_mode ? 3 : 2) * args->screen_width * args->screen_height;

    if (args->framebuffer < mapping_start) {
        init_video(args->framebuffer, args->screen_width, args->screen_height, args->is_graphic_mode);
        return mapping_start;
    }

    set_mapping(mapping_start, args->framebuffer, buffersz / 4096 + 1);
    init_video(mapping_start, args->screen_width, args->screen_height, args->is_graphic_mode);

    return mapping_start + buffersz;
}

short printstar(int irq) {
    if (! IRQ_FLAGS[irq]) {
        putchar('*');
        return 1;
    }
    while (true);
    return 1;
}

void entry(_IN struct boot_args *_args) {
    if (_args->magic != BOOT_ARGS_MAGIC) {
        while (true);
    }
    struct boot_args args;
    memcpy(&args, _args, sizeof(struct boot_args));

    init_kheap();
    init_gdt();

    SEGMENT_TOKEN KERNEL_TOKEN, KHEAP_TOKEN, PAGING_TOKEN;
    init_segments(args.kernel_start, args.kernel_limit, &KERNEL_TOKEN, &KHEAP_TOKEN);

    qword pmemsz = (((qword)args.memory_size_high) << 32) + args.memory_size;
    qword vmemsz = TO2POW(pmemsz * 2467 / 1525, MEMUNIT_SZ);

    init_paging(vmemsz, &PAGING_TOKEN);

    qword mapping_start = pmemsz;

    mapping_start = init_video_info(&args, mapping_start);

    set_print_color(0x0F);
    set_scroll_line(24);

    init_pic();

    init_idt();

    init_sse();

    init_pit(18.20679f);

    asmv ("sti");

    register_irq_handler(0, printstar);
    enable_irq(0);
}