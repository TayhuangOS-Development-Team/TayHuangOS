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
#include <tayhuang/descs.h>
#include <tayhuang/io.h>

#include "kheap.h"

#include "memory/paging.h"

#include "display/video.h"
#include "display/printk.h"

#include "intterup/init_int.h"
#include "intterup/irq_handler.h"

#include "init/init_clock.h"
#include "init/init_misc.h"

PRIVATE struct desc_struct GDT[8];
PRIVATE struct gdt_ptr gdtr;

void init_gdt(void) {
    int cs_idx = rdcs() >> 3;
    int ds_idx = rdds() >> 3;
    int tr_idx = 3;
    GDT[0] = (struct desc_struct)GDT_ENTRY(0, 0, 0);
    GDT[cs_idx] = (struct desc_struct)GDT_ENTRY(0xA09A, 0, 0xFFFFF);
    GDT[ds_idx] = (struct desc_struct)GDT_ENTRY(0xA093, 0, 0xFFFFF);
    GDT[tr_idx] = (struct desc_struct)GDT_ENTRY(0, 0, 0); //WIP
    gdtr.ptr = GDT;
    gdtr.len = sizeof (GDT);
    asmv ("lgdt %0" : : "m"(gdtr));
}

qword init_video_info(_IN struct boot_args *args, _IN qword mapping_start) {
    int buffersz = (args->is_graphic_mode ? 3 : 2) * args->screen_width * args->screen_height;

    if (args->framebuffer < mapping_start) {
        init_video(args->framebuffer, args->screen_width, args->screen_height, args->is_graphic_mode);
        return mapping_start;
    }

    set_mapping(mapping_start, args->framebuffer, buffersz / 4096 + 1, true, false);
    init_video(mapping_start, args->screen_width, args->screen_height, args->is_graphic_mode);

    return mapping_start + buffersz;
}

// void procA(void) {
//     while (true) {
//         putchar ('A');
//         flush_to_screen();
//     }
// }

// void procB(void) {
//     while (true) {
//         putchar ('B');
//         flush_to_screen();
//     }
// }

short printstar(int irq) {
    if (IRQ_FLAGS[irq] == 0) {
        putchar('*');
        flush_to_screen();
        return 1;
    }
    return IRQ_FLAGS[irq] - 1;
}

void entry(_IN struct boot_args *_args) {
    if (_args->magic != BOOT_ARGS_MAGIC) {
        while (true);
    }
    struct boot_args args;
    memcpy(&args, _args, sizeof(struct boot_args));

    init_kheap();
    init_gdt();

    qword pmemsz = (((qword)args.memory_size_high) << 32) + args.memory_size;
    qword vmemsz = TO2POW(pmemsz * 2467 / 1525, MEMUNIT_SZ);

    init_sse();

    qword pagingsz;

    void *pml4_start_address = init_paging(vmemsz, &pagingsz);

    set_mapping(0x100000, 0x100000, (args.kernel_limit - 0x100000) / 4096, true, false);
    set_mapping(pml4_start_address, pml4_start_address, pagingsz / 4096, true, false);

    cr3_t cr3 = get_cr3();
    cr3.page_entry = (qword)pml4_start_address;

    asmv ("xchg %bx, %bx");

    set_cr3(cr3);

    while (true);

    qword mapping_start = pmemsz;

    mapping_start = init_video_info(&args, mapping_start);

    set_print_color(0x0F);
    set_scroll_line(24);

    init_pic();

    init_idt();

    init_pit(18.20679f);

    asmv ("sti");

    register_irq_handler(0, printstar);
    enable_irq(0);
}