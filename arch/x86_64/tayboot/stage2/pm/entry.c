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
#include <descs.h>
#include "../printf.h"
#include "../intcall.h"
#include "vesa.h"
#include "../drivers/drivers.h"
#include "../drivers/memory/memory_driver.h"
#include <boot_args.h>
#include <ports.h>

#define EMPTY_DESC_NO (0)
#define CS_DESC_NO (1)
#define DS_DESC_NO (2)
#define TSS_DESC_NO (3)

PRIVATE void setup_gdt(void) {
    //一个临时的gdt
    PRIVATE const struct desc_struct BOOT_GDT[] __attribute__((aligned(16))) = {
        [EMPTY_DESC_NO] = GDT_ENTRY(0, 0, 0), //EMPTY
        [CS_DESC_NO] = GDT_ENTRY(0xC09A, 0, 0xFFFFF),
        [DS_DESC_NO] = GDT_ENTRY(0xC093, 0, 0xFFFFF),
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

PUBLIC void the_finally_jump(void* entrypoint, sreg_t cs_selector, sreg_t ds_selector, sreg_t tss_selector, void* boot_args);

struct boot_args boot_args;

PRIVATE int get_mem_size(void) {
    int size = 0;
    memory_driver.pc_handle(&memory_driver, MM_CMD_GET_MEM_SIZE, &size);
    return size;
}

PRIVATE void init_boot_args(void) {
    boot_args.magic = BOOT_ARGS_MAGIC;
    boot_args.memory_size = get_mem_size();
#ifdef ENABLE_GRAPHIC_BEFORE_GOTO_OS
    void* framebuffer = enable_graphic();
    if (framebuffer) {
        boot_args.is_graphic_mode = true;
        boot_args.screen_height = 1024;
        boot_args.screen_width = 768;
        boot_args.framebuffer = framebuffer;
    }
    else {
        printf ("Your vedio don't support vesa!");
#else
    boot_args.is_graphic_mode = false;
    boot_args.screen_height = 25;
    boot_args.screen_width = 80;
    boot_args.framebuffer = (void*)0xB8000;
#endif
#ifdef ENABLE_GRAPHIC_BEFORE_GOTO_OS
    }
#endif
}

PUBLIC void go_to_protect_mode(void) {
    init_boot_args();
    void* entrypoint = load_stage3();
    if (entrypoint == NULL) {
        printf ("Error! We can't load stage3!Or reboot?");
        return;
    }
    if (! enable_a20()) {
        printf ("Error! We can't enable the A20 gate!Or reboot?");
        return;
    }
    //关中断
    asmv ("cli");
    outb(CMOS_RAM_ADR, 0x80);
	io_delay();
    //重置协处理器
    outb(CO_CPU_0, 0);
	io_delay();
	outb(CO_CPU_1, 0);
	io_delay();
    //设置gdt
    setup_gdt();
    //设置idt
    setup_idt();
    //jump to protect mode
    the_finally_jump(entrypoint, CS_DESC_NO << 3, DS_DESC_NO << 3, TSS_DESC_NO << 3, (((dword)(&boot_args)) + (rdds() << 4)));
}