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

PRIVATE const qword BOOT_GDT[] = {
    GDT_ENTRY(0, 0, 0), //EMPTY
    GDT_ENTRY(0xC09b, 0, 0xFFFFF),
    GDT_ENTRY(0xC093, 0, 0xFFFFF),
    GDT_ENTRY(0x0089, 4096, 103),
};

PRIVATE struct gdt_ptr gdtr;
PRIVATE struct gdt_ptr null_idtr;

PRIVATE void setup_gdt(void) {
    gdtr.len = sizeof (BOOT_GDT) - 1;
    gdtr.ptr = (dword)&BOOT_GDT + ((dword)ds()) << 4;

    asmv ("lgdtl %0" : : "m"(gdtr));
}

PRIVATE void setup_idt(void) {
    null_idtr.len = 0;
    null_idtr.ptr = 0;
    asmv ("lidtl %0" : : "m"(null_idtr));
}

PUBLIC void go_to_protect_mode(void) {
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
    //设置gdt与idt
    setup_gdt();
    setup_idt();
    //jump to protect mode
    while (true); //pause here
}