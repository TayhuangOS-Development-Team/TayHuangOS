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

PRIVATE void init_gdt(void) {
    int cs_idx = rdcs() >> 3;
    int ds_idx = rdds() >> 3;
    int tr_idx = 3;
    GDT[0] = (struct desc_struct)GDT_ENTRY(0, 0, 0);
    GDT[cs_idx] = (struct desc_struct)GDT_ENTRY(0xA09A, 0, 0xFFFFF);
    GDT[ds_idx] = (struct desc_struct)GDT_ENTRY(0xA093, 0, 0xFFFFF);
    GDT[tr_idx] = (struct desc_struct)GDT_ENTRY(0x0089, ((qword)&TSS), sizeof(TSS));
    gdtr.ptr = GDT;
    gdtr.len = sizeof (GDT);
    asmv ("lgdt %0" : : "m"(gdtr));
    //asmv ("movq $0x18, %rax\n"
    //      "ltr %ax");
}

PRIVATE void init_video_info(_IN struct boot_args *args) {
    int buffersz = (args->is_graphic_mode ? 3 : 2) * args->screen_width * args->screen_height;

    set_mapping(args->framebuffer, args->framebuffer, buffersz / 4096 + 1, true, true);
    init_video(args->framebuffer, args->screen_width, args->screen_height, args->is_graphic_mode);
}

struct clock_intterup_args {
    b64 rflags,
        r15,
        r14,
        r13,
        r12,
        r11,
        r10,
        r9,
        r8,
        rdi,
        rsi,
        rdx,
        rcx,
        rbx,
        rax,
        rbp,
        rsp,
        cs,
        rip;
    b64 rflags2;
} __attribute__((packed));

void clock_int_handler(int irq, struct clock_intterup_args *regs) { //WIP
    disable_irq(irq);
    asmv ("sti");

    send_eoi(irq);

    while (true);

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
    kernel_pml4 = init_paging();
    set_pml4(kernel_pml4);
    qword kernel_length = args->kernel_limit - 0x100000;

    set_mapping(0, 0, pmemsz / 4096, true, true); //全部映射到自身
    set_mapping(0x100000, 0x100000, (kernel_length / 4096) + ((kernel_length % 4096) != 0), true, false); //KHEAP-KSTACK-KERNEL RW = TRUE, US = SUPER

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

    enable_irq(0);

    //do_kernel_fork(1, init);
}