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
#include <string.h>

#include "kheap.h"

#include "memory/paging.h"
#include "memory/pmm.h"

#include "display/video.h"
#include "display/printk.h"

#include "intterup/init_int.h"
#include "intterup/irq_handler.h"

#include "init/init_clock.h"
#include "init/init_misc.h"

#include "process/task_manager.h"

PRIVATE struct desc_struct GDT[8];
PRIVATE struct gdt_ptr gdtr;
PRIVATE struct tss TSS;

PRIVATE inline void set_tss(void *addr, qword base, dword limit, byte type) {
    struct tss_struct *desc = (struct tss_struct*)addr;
    memset(desc, 0, sizeof(struct tss_struct));

    desc->limit0 = (word)limit;
    desc->limit1 = (limit >> 16) & 0xF;
    desc->base0 = (word)base;
    desc->base1 = (base >> 16) & 0xFF;
    desc->base2 = (base >> 24) & 0xFF;
    desc->base3 = (dword)(base >> 32);
    desc->type = type;
    desc->p = 1;
}

#define cs0_idx (4)
#define cs3_idx (6)
#define ds0_idx (3)
#define ds3_idx (5)
#define tr_idx (1)

PRIVATE void init_gdt(void) {
    GDT[0] = (struct desc_struct)GDT_ENTRY(0, 0, 0);
    GDT[cs0_idx] = (struct desc_struct)GDT_ENTRY(0xA09A, 0, 0xFFFFF);
    GDT[cs3_idx] = (struct desc_struct)GDT_ENTRY(0xA0FA, 0, 0xFFFFF);
    GDT[ds0_idx] = (struct desc_struct)GDT_ENTRY(0xA093, 0, 0xFFFFF);
    GDT[ds3_idx] = (struct desc_struct)GDT_ENTRY(0xA0F3, 0, 0xFFFFF);

    set_tss(&GDT[tr_idx], &TSS, sizeof(TSS), DESC_TSS);

    gdtr.ptr = GDT;
    gdtr.len = sizeof (GDT);
    asmv ("lgdt %0" : : "m"(gdtr));
    stds(ds0_idx << 3);
    stes(ds0_idx << 3);
    stfs(ds0_idx << 3);
    stgs(ds0_idx << 3);
    stss(ds0_idx << 3);
    int _tr_idx = tr_idx << 3;
    asmv ("ltr %0" : : "m"(_tr_idx));
}

PRIVATE void init_video_info(_IN struct boot_args *args) {
    int buffersz = (args->is_graphic_mode ? 3 : 2) * args->screen_width * args->screen_height;

    set_mapping(args->framebuffer, args->framebuffer, buffersz / 4096 + 1, true, true);
    init_video(args->framebuffer, args->screen_width, args->screen_height, args->is_graphic_mode);
}

void printA(void) {
    while (true) {
        putchar ('A');
        flush_to_screen();
        for (int i = 0 ; i < 750000 ; i ++);
    }
}

void printB(void) {
    asmv ("xchg %bx, %bx");
    while (true) {
        putchar ('B');
        flush_to_screen();
        for (int i = 0 ; i < 750000 ; i ++);
    }
}

void clock_int_handler(int irq, struct intterup_args *regs) {
    disable_irq(irq);
    asmv ("sti");

    send_eoi(irq);

    if (current_task != NULL) {
        if (current_task->counter <= 0) {
            do_switch(regs);
        }
        current_task->counter --;
    }

    asmv ("cli");
    enable_irq(irq);
}

#define MMIO_START (0x30000000000)

PRIVATE void *kernel_pml4 = NULL;

void init(void) {
    printk ("Hello, World!I'm init, the first process!\n");
    while (true);
}

#define CLOCK_FREQUENCY (18.20679f)

void initialize(_IN struct boot_args *args) {
    init_gdt();

    qword pmemsz = (((qword)args->memory_size_high) << 32) + args->memory_size;

    init_kheap(args->kernel_start - 0x40000);
    init_pmm(pmemsz);
    mark_used(0);
    for (int i = 0xA0000 ; i < args->kernel_limit ; i += 4096) {
        mark_used(i);
    }

    //vmem map
    //----------------------------------------------------------------------------------------
    //|  FREE   |   KHEAP   |  STACK  |  KERNEL  |  PAGING  |      FREE       |     MMIO     |
    //----------------------------------------------------------------------------------------
    kernel_pml4 = create_pgd();
    set_pml4(kernel_pml4);
    qword kernel_length = args->kernel_limit - 0x100000;

    set_mapping(0, 0, pmemsz / 4096, true, true); //全部映射到自身
    set_mapping(0x100000, 0x100000, (kernel_length / 4096) + ((kernel_length % 4096) != 0), true, true); //KHEAP-KSTACK-KERNEL RW = TRUE, US = SUPER

    cr3_t cr3 = get_cr3();
    cr3.page_entry = (qword)kernel_pml4; //设置CR3
    set_cr3(cr3);

    init_video_info(args);

    set_print_color(0x0F);
    set_scroll_line(15);

    init_sse();

    init_pit(CLOCK_FREQUENCY);

    init_pic();
    init_idt();

    asmv ("sti");
}

void entry(_IN struct boot_args *_args) {
    //Boot Arg
    if (_args->magic != BOOT_ARGS_MAGIC) {
        while (true);
    }
    struct boot_args args;
    memcpy(&args, _args, sizeof(struct boot_args));

    initialize(&args);

    TSS.ist1 = 0x140000;
    TSS.rsp0 = 0x125000;

    enable_irq(0);

    create_task(1, printB, (1 << 9), 0x135000, cs3_idx << 3 | 3, get_pml4());
    create_task(1, printA, 1 << 9, 0x130000, rdcs(), get_pml4());
    current_task = create_task(1, init, 1 << 9, 0x125000, rdcs(), get_pml4());
    current_task->counter = 19;

    asmv ("movq $0x125000, %rsp");
    asmv ("jmp init");
}