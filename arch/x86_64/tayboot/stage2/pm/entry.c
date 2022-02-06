/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/pm/entry.c
 *
 * 保护模式入口
 */



#include "entry.h"
#include "a20.h"
#include <tayboot/descs.h>
#include "../printf.h"
#include "../intcall.h"
#include "vesa.h"
#include "../drivers/drivers.h"
#include "../drivers/memory/memory_driver.h"
#include <tayboot/stage3_args.h>
#include <tayboot/ports.h>

#define EMPTY_DESC_NO (0) //空描述符
#define CS_DESC_NO (1) //代码段描述符
#define DS_DESC_NO (2) //数据段描述符
#define TSS_DESC_NO (3) //TSS描述符

PRIVATE void setup_gdt(void) {
    //一个临时的gdt
    PRIVATE const struct desc_struct BOOT_GDT[] __attribute__((aligned(16))) = {
        [EMPTY_DESC_NO] = GDT_ENTRY(0, 0, 0), //EMPTY
        [CS_DESC_NO] = GDT_ENTRY(0xC09A, 0, 0xFFFFF), //代码段
        [DS_DESC_NO] = GDT_ENTRY(0xC093, 0, 0xFFFFF), //数据段
        [TSS_DESC_NO] = GDT_ENTRY(0x0089, 4096, 103) //没用的TSS，用于欺骗CPU
    };

    PRIVATE struct gdt_ptr gdtr;
    gdtr.len = sizeof (BOOT_GDT) - 1;
    gdtr.ptr = (dword)&BOOT_GDT + (((dword)rdds()) << 4);

    asmv ("lgdtl %0" : : "m"(gdtr));
}

PRIVATE void setup_idt(void) {
    PRIVATE struct gdt_ptr idtr; //相同结构
    idtr.len = 0;
    idtr.ptr = 0;
    //欺骗CPU
    asmv ("lidtl %0" : : "m"(idtr));
}

PUBLIC void the_finally_jump(void *entrypoint, sreg_t cs_selector, sreg_t ds_selector, sreg_t tss_selector, void *stage3_args); //最终一跳

struct stage3_args stage3_args; //STAGE3引导参数

PRIVATE int get_mem_size(void) {
    int size = 0;
    memory_driver.pc_handle(&memory_driver, MM_CMD_GET_MEM_SIZE, &size);
    return size;
}

PRIVATE void init_stage3_args(void) {
    stage3_args.magic = STAGE3_ARGS_MAGIC;
    stage3_args.memory_size = get_mem_size();
    stage3_args.memory_size_high = get_memsz_high();
#ifdef ENABLE_GRAPHIC_BEFORE_GOTO_OS //启用图形界面
    void *framebuffer = enable_graphic();
    if (framebuffer) {
        stage3_args.is_graphic_mode = true;
        stage3_args.screen_height = 768;
        stage3_args.screen_width = 1024;
        stage3_args.framebuffer = framebuffer;
    }
    else { //不支持vesa
        printf ("Your vedio don't support vesa!");
#else
    stage3_args.is_graphic_mode = false;
    stage3_args.screen_height = 25;
    stage3_args.screen_width = 80;
    stage3_args.framebuffer = (void*)0xB8000;
#endif
#ifdef ENABLE_GRAPHIC_BEFORE_GOTO_OS
    }
#endif
}

PUBLIC void go_to_protect_mode(void) { //去到保护模式
    init_stage3_args(); //初始化引导参数
    void *entrypoint = load_stage3(); //加载stage3并获取入口点
    if (entrypoint == NULL) { //无法加载
        printf ("Error! We can't load stage3!Or reboot?");
        return;
    }
    if (! enable_a20()) { //无法打开A20
        printf ("Error! We can't enable the A20 gate!Or reboot?");
        return;
    }
    //关中断
    asmv ("cli");
    outb(CMOS_RAM_ADDRESS, 0x80);
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
    //保护模式 我来力
    the_finally_jump(entrypoint, CS_DESC_NO << 3, DS_DESC_NO << 3, TSS_DESC_NO << 3, (((dword)(&stage3_args)) + (rdds() << 4)));
}