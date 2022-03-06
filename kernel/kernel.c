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

    set_mapping(mapping_start, args->framebuffer, buffersz / 4096 + 1, true, true);
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

#define MMIO_START (0x30000000000)

void entry(_IN struct boot_args *_args) {
    //Boot Arg
    if (_args->magic != BOOT_ARGS_MAGIC) {
        while (true);
    }
    struct boot_args args;
    memcpy(&args, _args, sizeof(struct boot_args));

    //Memory
    init_gdt();

    qword pmemsz = (((qword)args.memory_size_high) << 32) + args.memory_size;
    qword vmemsz = TO2POW(pmemsz * 2467 / 1525, MEMUNIT_SZ);

    init_sse(); //SSE

    qword pagingsz;

    void *pml4_start_address = init_paging(vmemsz, (args.kernel_limit & 0xFFFFFFFFFFFFF000) + 0x1000, &pagingsz);

    set_mapping(0x0, 0x0, pmemsz / 4096, true, false); //全部映射至原来位置

    qword mapping_start = MMIO_START;

    mapping_start = init_video_info(&args, mapping_start); //MMIO映射到高处(显存)

    set_mapping(args.kernel_start - 0x40000, args.kernel_start - 0x40000, (args.kernel_limit - args.kernel_start + 0x40000) / 4096, true, false); //内核&栈

    set_mapping(0xA0000, (pml4_start_address + pagingsz), 0x60000 / 4096, true, false); //KHEAP2
    set_mapping(pml4_start_address, pml4_start_address, pagingsz / 4096, true, false); //页表

    cr3_t cr3 = get_cr3();
    cr3.page_entry = (qword)pml4_start_address; //设置CR3
    set_cr3(cr3);

    init_kheap(args.kernel_start - 0x40000); //0x00001 ~ args.kernel_start - 0x40000: KHEAP
    //vmem map
    //------------------------------------------------------------------------------
    //|   KHEAP   |  STACK  |  KERNEL  |  PAGING  |      FREE       |     MMIO     |
    //------------------------------------------------------------------------------

    set_print_color(0x0F); //设置显示信息
    set_scroll_line(24);

    printk ("Hello, World!\n");

    //中断

    init_pic();

    init_idt();

    init_pit(18.20679f);

    asmv ("sti");

    register_irq_handler(0, printstar); //时钟中断
    enable_irq(0);
}