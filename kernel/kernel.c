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
#include <assert.h>

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

#include "syscall/syscall.h"
#include "syscall/system_api.h"

#include "test/test_proccess.h"

PRIVATE struct desc_struct GDT[8];
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

#define cs0_idx (4)
#define cs3_idx (6)
#define ds0_idx (3)
#define ds3_idx (5)
#define tr_idx (1)

PRIVATE void init_gdt(void) { //初始化GDT
    GDT[0] = (struct desc_struct)GDT_ENTRY(0, 0, 0);
    GDT[cs0_idx] = (struct desc_struct)GDT_ENTRY(0xA09A, 0, 0xFFFFF);
    GDT[cs3_idx] = (struct desc_struct)GDT_ENTRY(0xA0FA, 0, 0xFFFFF);
    GDT[ds0_idx] = (struct desc_struct)GDT_ENTRY(0xA092, 0, 0xFFFFF);
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

PRIVATE void init_video_info(_IN struct boot_args *args) { //初始化视频
    int buffersz = (args->is_graphic_mode ? 3 : 2) * args->screen_width * args->screen_height;

    set_mapping(args->framebuffer, args->framebuffer, buffersz / 4096 + 1, true, true); //初始化显存页表
    init_video(args->framebuffer, args->screen_width, args->screen_height, args->is_graphic_mode); //初始化视频
}

PRIVATE bool stop_switch = false;

PUBLIC void after_syscall(struct intterup_args *regs) {
    if (stop_switch)
        return;
    if (current_task != NULL) {
        if ((current_task->counter <= 0) || (current_task->state != RUNNING) || (current_task->state != READY)) { //不符合继续运行的条件
            do_switch(regs); //切换
        }
    }
}

PUBLIC short clock_int_handler(int irq, struct intterup_args *regs, bool entered_handler);

#define MMIO_START (0x30000000000)

PRIVATE void *kernel_pml4 = NULL;

#define CS_USER (cs3_idx << 3 | 3)
#define RFLAGS_USER ((1 << 9) | (3 << 12))
#define CS_KERNEL (cs0_idx << 3)
#define RFLAGS_KERNEL (1 << 9)

#define API_PID(x) (0x10000 + (x))

void init(void) { //init进程
    stop_switch = false;
    printk ("\n");

    void *level3_pml4 = create_pgd(); //用户级页表
    set_pml4(level3_pml4);
    set_mapping(0, 0, 16384, true, true);

    //API PROCCESS
    //create_task(1, clock_api_process, RFLAGS_KERNEL, 0x1100000, CS_KERNEL, kernel_pml4)->pid = API_PID(0);
    //create_task(1, video_api_process, RFLAGS_KERNEL, 0x1150000, CS_KERNEL, kernel_pml4)->pid = API_PID(1);

    //TEST PROCCESS
    //create_task(2, keyboard_handler, RFLAGS_KERNEL, 0x1350000, CS_KERNEL, kernel_pml4);
    //create_task(1, fake_shell, RFLAGS_USER, 0x1300000, CS_USER, level3_pml4);
    //create_task(2, tick_display, RFLAGS_USER, 0x1200000, CS_USER, level3_pml4);
    create_task(1, __test_proc1, RFLAGS_USER, 0x1300000, CS_USER, level3_pml4);
    //create_task(1, __test_proc2, RFLAGS_USER, 0x1200000, CS_USER, level3_pml4);
    //exit();
    while (true);
}

#define CLOCK_FREQUENCY (50.0f) //时钟周期

void initialize(_IN struct boot_args *args) {
    init_gdt(); //初始化GDT

    qword pmemsz = (((qword)args->memory_size_high) << 32) + args->memory_size; //计算物理内存大小

    init_kheap(args->kernel_start - 0x40000);
    init_pmm(pmemsz);
    mark_used(0); //第一个0-4KB不能使用
    for (int i = 0xA0000 ; i < args->kernel_limit ; i += 4096) {
        mark_used(i); //显存不能使用
    }

    //vmem map
    //----------------------------------------------------------------------------------------
    //|  FREE   |   KHEAP   |  STACK  |  KERNEL  |  PAGING  |      FREE       |     MMIO     |
    //----------------------------------------------------------------------------------------
    kernel_pml4 = create_pgd(); //内核页表
    set_pml4(kernel_pml4);
    qword kernel_length = args->kernel_limit - 0x100000; //内核区大小

    set_mapping(0, 0, pmemsz / 4096, true, false); //全部映射到自身
    set_mapping(0x1000000, 0x1000000, (kernel_length / 4096) + ((kernel_length % 4096) != 0), true, false); //KHEAP-KSTACK-KERNEL RW = TRUE, US = SUPER

    cr3_t cr3 = get_cr3();
    cr3.page_entry = (qword)kernel_pml4; //设置CR3
    set_cr3(cr3);

    init_video_info(args);

    set_print_color(0x0F);
    set_scroll_line(15);

    init_sse(); //SSE

    init_pit(CLOCK_FREQUENCY); //PIT

    init_pic(); //PIC
    init_idt(); //IDT

    en_int();

    init_keyboard(); //键盘
}

void entry(_IN struct boot_args *_args) {
    //Boot Arg
    if (_args->magic != BOOT_ARGS_MAGIC) {
        while (true);
    }
    struct boot_args args;
    memcpy(&args, _args, sizeof(struct boot_args));

    initialize(&args); //初始化

    TSS.ist1 = 0x1400000;
    TSS.rsp0 = 0x1250000;
    current_task = create_task(1, init, RFLAGS_KERNEL, 0x1250000, CS_KERNEL, get_pml4()); //init进程
    current_task->counter = 1;

    stop_switch = true;

    register_irq_handler(0, clock_int_handler);
    register_irq_handler(1, keyboard_int_handler); //中断处理器

    asmv ("movq $0x125000, %rsp"); //设置堆
    enable_irq(0); //开启时钟中断
    enable_irq(1);
    asmv ("jmp init"); //跳转至INIT进程
}