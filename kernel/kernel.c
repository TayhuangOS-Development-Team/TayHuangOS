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
#include <tayhuang/services.h>
#include <tayhuang/control_registers.h>

#include <string.h>
#include <assert.h>

#include <memory/kheap.h>
#include <memory/pmm.h>

#include <intterup/init_int.h>
#include <intterup/clock/clock.h>

#include <printk.h>
#include <logging.h>

PRIVATE struct desc_struct GDT[16];
PRIVATE struct gdt_ptr gdtr;
PRIVATE struct tss TSS;

PRIVATE inline void set_tss(void *addr, qword base, dword limit, byte type) { //设置TSS
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

#define tr_idx (1)
#define cs3_idx (3)
#define ds3_idx (4)
#define cs1_idx (5)
#define ds1_idx (6)
#define cs0_idx (7)
#define ds0_idx (8)

PRIVATE void init_gdt(void) { //初始化GDT
    GDT[0] = (struct desc_struct)GDT_ENTRY(0, 0, 0);
    GDT[cs0_idx] = (struct desc_struct)GDT_ENTRY(0xA09A, 0, 0xFFFFF);
    GDT[cs1_idx] = (struct desc_struct)GDT_ENTRY(0xA0BA, 0, 0xFFFFF);
    GDT[cs3_idx] = (struct desc_struct)GDT_ENTRY(0xA0FA, 0, 0xFFFFF);
    GDT[ds0_idx] = (struct desc_struct)GDT_ENTRY(0xA092, 0, 0xFFFFF);
    GDT[ds1_idx] = (struct desc_struct)GDT_ENTRY(0xA0B2, 0, 0xFFFFF);
    GDT[ds3_idx] = (struct desc_struct)GDT_ENTRY(0xA0F2, 0, 0xFFFFF);

    set_tss(&GDT[tr_idx], &TSS, sizeof(TSS), DESC_TSS); //设置TSS

    gdtr.ptr = GDT;
    gdtr.len = sizeof (GDT);
    asmv ("lgdt %0" : : "m"(gdtr)); //加载GDT
    stds(ds0_idx << 3); //初始化段寄存器
    stes(ds0_idx << 3);
    stfs(ds0_idx << 3);
    stgs(ds0_idx << 3);
    stss(ds0_idx << 3);
    int _tr_idx = tr_idx << 3; //初始化TR寄存器
    asmv ("ltr %0" : : "m"(_tr_idx));
}

static inline void init_sse(void) { //SSE
    cr0_t cr0 = get_cr0();
    cr0.em = 0;
    cr0.mp = 1;
    set_cr0(cr0);

    cr4_t cr4 = get_cr4();
    cr4.osfxsr = 1;
    cr4.osxmmexcpt = 1;
    set_cr4(cr4);
}

PUBLIC void *kernel_pml4 = NULL;

#define CS_USER (cs3_idx << 3 | 3)
#define RFLAGS_USER ((1 << 9) | (3 << 12))
#define CS_SERVICE (cs1_idx << 3 | 1)
#define RFLAGS_SERVICE ((1 << 9) | (1 << 12))
#define CS_KERNEL (cs0_idx << 3)
#define RFLAGS_KERNEL (1 << 9)

PRIVATE struct boot_args args;

#define CLOCK_FREQUENCY (50.0f) //时钟周期

#define SETUP_MOD_BASE (64 * 1024)
#define SETUP_MOD_SIZE (320 * 1024)
#define KHEAP_PRE_BASE (SETUP_MOD_BASE + SETUP_MOD_SIZE)
#define KHEAP_PRE_SIZE (640 * 1024 - KHEAP_PRE_BASE)

void initialize(struct boot_args *args) {
    init_gdt(); //初始化GDT

    en_int();

    init_serial();

    init_video(args->framebuffer, args->screen_width, args->screen_height);

    memcpy (SETUP_MOD_BASE, args->setup_mod_addr, SETUP_MOD_SIZE);

    init_kheap(KHEAP_PRE_BASE, KHEAP_PRE_SIZE);

    qword memsz = (((qword)args->memory_size_high) << 32) + args->memory_size;

    init_pmm(memsz, 0x2000000);

    init_idt();
    init_pic();

    init_sse();
    init_pit(CLOCK_FREQUENCY);

    TSS.ist1 = 0x600000;
    TSS.rsp0 = 0x500000;
}

void entry(struct boot_args *_args) {
    //Boot Arg
    if (_args->magic != BOOT_ARGS_MAGIC) {
        while (true);
    }
    memcpy(&args, _args, sizeof(struct boot_args));

    initialize(&args); //初始化

    linfo ("Kernel", "Hello, I'm %s!", "TayhuangOS Kernel");

    while(true);
}