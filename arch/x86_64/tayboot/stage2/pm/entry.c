// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/pm/entry.c
 *
 * Protect Mode Entry here
 */



#include "entry.h"
#include "a20.h"
#include "gdt.h"
#include "../printf.h"

#define EMPTY_DESC_NO (0)
#define CS_DESC_NO (1)
#define DS_DESC_NO (2)
#define TSS_DESC_NO (3)

PRIVATE void setup_gdt(void) {
    //一个临时的gdt
    PRIVATE const struct desc_struct BOOT_GDT[] __attribute__((aligned(16))) = {
        [EMPTY_DESC_NO] = GDT_ENTRY(0, 0, 0), //EMPTY
        [CS_DESC_NO] = GDT_ENTRY(0x409A, 0, 0xFFFFF),
        [DS_DESC_NO] = GDT_ENTRY(0x4093, 0, 0xFFFFF),
        [TSS_DESC_NO] = GDT_ENTRY(0x0089, 4096, 103) //没用，用于欺骗CPU
    };

    PRIVATE struct gdt_ptr gdtr;
    gdtr.len = sizeof (BOOT_GDT) - 1;
    gdtr.ptr = (dword)&BOOT_GDT + (((dword)rdds()) << 4);

    asmv ("lgdtl %0" : : "m"(gdtr));
}

PRIVATE void setup_idt(void) {
    PRIVATE struct gdt_ptr idtr;
    idtr.len = 0;
    idtr.ptr = 0;
    //欺骗CPU
    asmv ("lidtl %0" : : "m"(idtr));
}

void the_finally_jump(dword entrypoint, sreg_t cs_selector, sreg_t ds_selector, sreg_t tss_selector);

PUBLIC void go_to_protect_mode(dword entrypoint) {
    if (! enable_a20()) {
        printf ("Error! We can't enable the A20 gate!Or reboot?");
        while (true);
    }
    //关中断
    asmv ("cli");
    outb(0x70, 0x80);
	io_delay();
    //重置协处理器
    outb(0xF0, 0);
	io_delay();
	outb(0xF1, 0);
	io_delay();
    //设置gdt
    setup_gdt();
    //设置idt
    setup_idt();
    //jump to protect mode
    the_finally_jump(entrypoint, CS_DESC_NO << 3, DS_DESC_NO << 3, TSS_DESC_NO << 3);
}