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

#include "keyboard/keyboard.h"

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
    GDT[ds0_idx] = (struct desc_struct)GDT_ENTRY(0xA092, 0, 0xFFFFF);
    GDT[ds3_idx] = (struct desc_struct)GDT_ENTRY(0xA0F2, 0, 0xFFFFF);

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

#define CLOCK_FREQUENCY (500.0f)

PRIVATE volatile int ticks = 0;

void delay(int wait_ticks) {
    int last_ticks = ticks;
    while (ticks - last_ticks < wait_ticks);
}

char *getline(char *buffer) {
    char *result = buffer;
    char ch = getchar();
    while (ch != '\n') {
        *buffer = ch;
        ch = getchar();
        buffer ++;
    }
    *buffer = '\0';
    return result;
}

bool start_with(const char *str, const char *prefix) {
    int len = strlen(prefix);
    for (int i = 0 ; i < len ; i ++) {
        if (str[i] != prefix[i])
            return false;
    }
    return true;
}

void fake_shell(void) {
    char buffer[64];
    while (true) {
        int color = get_print_color();
        set_print_color(0x0A);
        putchar ('>');
        set_print_color(color);
        flush_to_screen();
        getline (buffer);
        if (start_with(buffer, "echo ")) {
            int color = get_print_color();
            set_print_color(0x0A);
            int len = strlen(buffer);
            for (int i = 5 ; i < len ; i ++) {
                putchar (buffer[i]);
            }
            set_print_color(color);
            putchar ('\n');
        }
        else {
            printk ("Wrong Input!\n");
        }
    }
}

void keyboard_handler(void) {
    while (true) {
        deal_key();
    }
}

void tick_display(void) {
    while (true) {
        int posx = get_pos_x(), posy = get_pos_y();
        change_pos(0, 0);
        int color = get_print_color();
        set_print_color(0x0D);
        printk ("Current Startup Time(s): %d\n", ticks / 500);
        set_print_color(color);
        change_pos(posx, posy);
        delay(250 * 2);
    }
}

void after_syscall(struct intterup_args *regs) {
    if (current_task != NULL) {
        if (current_task->counter <= 0) {
            do_switch(regs);
        }
    }
}

short clock_int_handler(int irq, struct intterup_args *regs, bool entered_handler) {
    ticks ++;
    if (! entered_handler)
        after_syscall(regs);
    current_task->counter --;
    return 0;
}

#define MMIO_START (0x30000000000)

PRIVATE void *kernel_pml4 = NULL;

void init(void) {
    printk ("\n");

    void *level3_pml4 = create_pgd();
    set_pml4(level3_pml4);
    set_mapping(0, 0, 16384, true, true);

    create_task(1, keyboard_handler, (1 << 9), 0x1350000, rdcs(), kernel_pml4);
    create_task(1, fake_shell, (1 << 9), 0x1300000, rdcs(), kernel_pml4);
    create_task(1, tick_display, (1 << 9), 0x1200000, rdcs(), kernel_pml4);
    while (true);
}

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

    set_mapping(0, 0, pmemsz / 4096, true, false); //全部映射到自身
    set_mapping(0x1000000, 0x1000000, (kernel_length / 4096) + ((kernel_length % 4096) != 0), true, false); //KHEAP-KSTACK-KERNEL RW = TRUE, US = SUPER

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

    en_int();

    init_keyboard();
}

void entry(_IN struct boot_args *_args) {
    //Boot Arg
    if (_args->magic != BOOT_ARGS_MAGIC) {
        while (true);
    }
    struct boot_args args;
    memcpy(&args, _args, sizeof(struct boot_args));

    initialize(&args);

    TSS.ist1 = 0x1400000;
    TSS.rsp0 = 0x1250000;
    current_task = create_task(1, init, (1 << 9), 0x1250000, rdcs(), get_pml4());
    current_task->counter = 9;

    register_irq_handler(0, clock_int_handler);
    register_irq_handler(1, keyboard_int_handler);

    asmv ("movq $0x125000, %rsp");
    enable_irq(0);
    enable_irq(1);
    asmv ("jmp init");
}