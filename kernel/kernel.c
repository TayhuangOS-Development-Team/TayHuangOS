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

#include <kheap.h>

#include <memory/paging.h>
#include <memory/pmm.h>
#include <memory/shared_memory.h>
#include <memory/mm_malloc.h>

#include <display/video.h>
#include <display/printk.h>

#include <intterup/init_int.h>
#include <intterup/irq_handler.h>

#include <init/init_clock.h>
#include <init/init_misc.h>

#include <process/task_manager.h>

#include <syscall/syscall.h>

#include <test/test_proccess.h>

#include <video/video.h>
#include <clock/clock.h>
#include <keyboard/keyboard.h>

#include <debug/logging.h>
#include <assert.h>

#include <kmod/kmod_loader.h>


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

PRIVATE void init_video_info(struct boot_args *args) { //初始化视频
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

PUBLIC void *kernel_pml4 = NULL;

#define CS_USER (cs3_idx << 3 | 3)
#define RFLAGS_USER ((1 << 9) | (3 << 12))
#define CS_KERNEL (cs0_idx << 3)
#define RFLAGS_KERNEL (1 << 9)

#define API_PID(x) (0x10000 + (x))

PRIVATE struct boot_args args;

void print_mod_info(program_info *mod_info) {
    char buffer[256];
    linfo ("--------------------------------------------");
    sprintk (buffer, "Start: %P", mod_info->start);
    linfo (buffer);
    sprintk (buffer, "Limit: %P", mod_info->limit);
    linfo (buffer);
    sprintk (buffer, "Entry: %P", mod_info->entry);
    linfo (buffer);
    sprintk (buffer, "PGD: %P", mod_info->pgd);
    linfo (buffer);
    sprintk (buffer, "Stack Bottom: %P", mod_info->stack_bottom);
    linfo (buffer);
    sprintk (buffer, "Stack Top: %P", mod_info->stack_top);
    linfo (buffer);
    sprintk (buffer, "Heap Bottom: %P", mod_info->heap_bottom);
    linfo (buffer);
    sprintk (buffer, "Heap Top: %P", mod_info->heap_top);
    linfo (buffer);
    linfo ("--------------------------------------------");
}

void empty(void) {
    while (true); //什么都不做
}

bool load_kmod_bysetup(const char *name, int size, program_info *info) {
    byte *addr = cpmalloc(size); //获取存放mm的地址
    char buffer[256];

    sprintk (buffer, "Buffer In %P", addr);
    linfo (buffer);

    shm_mapping(addr, 16, current_task->pid, API_PID(0)); //与setup共享

    send_msg(name, API_PID(0), strlen(name), 50);
    send_msg(&addr, API_PID(0), sizeof(addr), 50);

    bool status = false;
    receive_msg(&status, API_PID(0));

    if (! status) { //无法加载
        return false;
    }

    *info = load_kmod_from_memory(addr);

    cpfree(addr, size); //释放
    return true;
}

void init(void) { //init进程 代表内核
    program_info setup_mod_info = load_kmod_from_memory((void*)args.setup_mod_addr);

    char buffer[256];

    print_mod_info(&setup_mod_info);

    void *level3_pml4 = create_pgd(); //用户级页表
    set_pml4(level3_pml4);
    set_mapping(0, 0, 16384, true, true);

    empty_task = create_task(2,
        -1, empty, RFLAGS_KERNEL,
        0x1400000, 0x1399900,
        CS_KERNEL, kernel_pml4,
        0x1399900, args.kernel_limit,
        args.kernel_limit, args.kernel_limit + 0x100
    );

    create_task(API_PID(2),
        1, taskman, RFLAGS_KERNEL,
        0x1300000, 0x1250000,
        CS_KERNEL, kernel_pml4,
        0x1250000, args.kernel_limit,
        args.kernel_limit + 0x2000, args.kernel_limit + 0x3000
    );

    create_task(API_PID(0),
        1, setup_mod_info.entry, RFLAGS_KERNEL,
        setup_mod_info.stack_top, setup_mod_info.stack_bottom,
        CS_KERNEL, setup_mod_info.pgd,
        setup_mod_info.start, setup_mod_info.limit,
        setup_mod_info.heap_bottom, setup_mod_info.heap_top
    ); //SETUP MODULE 内核模块进程

    stop_switch = false;
    printk ("\n");

    send_msg(&current_task->pid, API_PID(0), sizeof(current_task->pid), 20);

    #define MM_SIZE (16 * MEMUNIT_SZ)
    program_info mm_mod_info;
    bool status = load_kmod_bysetup("mm.mod", MM_SIZE, &mm_mod_info);

    if (!status) {
        lerror ("Can't load MM!");
        panic ("Can't load MM!");
    }

    print_mod_info(&mm_mod_info);
    
    create_task(API_PID(1),
        1, mm_mod_info.entry, RFLAGS_KERNEL,
        mm_mod_info.stack_top, mm_mod_info.stack_bottom,
        CS_KERNEL, mm_mod_info.pgd,
        mm_mod_info.start, mm_mod_info.limit,
        mm_mod_info.heap_bottom, mm_mod_info.heap_top
    ); //MM MODULE 内核模块进程
    
    #define VIDEO_SIZE (16 * MEMUNIT_SZ)
    program_info video_mod_info;
    status = load_kmod_bysetup("video.mod", MM_SIZE, &video_mod_info);
    
    if (!status) {
        lerror ("Can't load video driver!");
        panic ("Can't load video driver!");
    }

    print_mod_info(&video_mod_info);
    
    create_task(API_PID(3),
        1, video_mod_info.entry, RFLAGS_KERNEL,
        video_mod_info.stack_top, video_mod_info.stack_bottom,
        CS_KERNEL, video_mod_info.pgd,
        video_mod_info.start, video_mod_info.limit,
        video_mod_info.heap_bottom, video_mod_info.heap_top
    ); //VIDEO DRIVER 内核模块进程

    shm_mapping(0xB8000, 16, current_task->pid, API_PID(3)); //将显存共享给VIDEO DRIVER

    enum {
        DISP_MODE_NONE = 0,
        DISP_MODE_TEXT,
        DISP_MODE_GRAPHIC
    };

    int mode = VIDEO_INFO.graphic ? DISP_MODE_GRAPHIC : DISP_MODE_TEXT;

    send_msg(&mode, API_PID(3), sizeof(mode), 20);

    qword cmd[] = {(1 << 28) | 0, 'a', 0, 0};

    send_msg(&cmd, API_PID(3), sizeof(cmd), 20);

    while (true);

    exit();
}

#define CLOCK_FREQUENCY (50.0f) //时钟周期

PRIVATE qword kernel_length;

PUBLIC void mapping_kernel(void) {
    set_mapping(0x1300000, 0x1300000, (kernel_length / 4096) + ((kernel_length % 4096) != 0), true, false); //KHEAP-KSTACK-KERNEL RW = TRUE, US = SUPER
}

void initialize(struct boot_args *args) {
    init_gdt(); //初始化GDT
    init_serial();

    qword pmemsz = (((qword)args->memory_size_high) << 32) + args->memory_size; //计算物理内存大小

    init_kheap(args->kernel_start - 0x40000); //初始化内核堆
    init_pmm(pmemsz); //初始化PMM
    for (int i = 0x00000 ; (i < args->kernel_limit + 0x3000) ; i += 4096) { //0~内核limit 不可用
        mark_used(i);
    }

    //vmem map
    //----------------------------------------------------------------------------------------
    //|  FREE   |   KHEAP   |  STACK  |  KERNEL  |  PAGING  |      FREE       |     MMIO     |
    //----------------------------------------------------------------------------------------
    kernel_pml4 = create_pgd(); //内核页表
    set_pml4(kernel_pml4);
    kernel_length = args->kernel_limit - 0x1300000; //内核区大小

    set_mapping(0, 0, pmemsz / 4096, true, false); //全部映射到自身
    mapping_kernel();

    cr3_t cr3 = get_cr3();
    cr3.page_entry = (qword)kernel_pml4; //设置CR3
    set_cr3(cr3);

    init_video_info(args); //初始化视频

    set_print_color(0x0F);
    set_scroll_line(15);

    init_sse(); //SSE

    init_pit(CLOCK_FREQUENCY); //PIT

    init_pic(); //PIC
    init_idt(); //IDT

    en_int();

    init_keyboard(); //键盘
}

void entry(struct boot_args *_args) {
    //Boot Arg
    if (_args->magic != BOOT_ARGS_MAGIC) {
        while (true);
    }
    memcpy(&args, _args, sizeof(struct boot_args));

    initialize(&args); //初始化

    TSS.ist1 = 0x1400000;
    TSS.rsp0 = 0x1250000;
    current_task = create_task(1, 1, init, RFLAGS_KERNEL, 0x1350000, 0x1300000, CS_KERNEL, get_pml4(), 0x1300000, args.kernel_limit, args.kernel_limit + 0x800, args.kernel_limit + 0x1800); //init进程
    current_task->counter = 1;

    stop_switch = true;

    register_irq_handler(0, clock_int_handler);
    register_irq_handler(1, keyboard_int_handler);
    register_irq_handler(2, wakeup_irq_handler);
    register_irq_handler(3, wakeup_irq_handler);
    register_irq_handler(4, wakeup_irq_handler);
    register_irq_handler(5, wakeup_irq_handler);
    register_irq_handler(6, wakeup_irq_handler);
    register_irq_handler(7, wakeup_irq_handler);
    register_irq_handler(8, wakeup_irq_handler);
    register_irq_handler(9, wakeup_irq_handler);
    register_irq_handler(10, wakeup_irq_handler);
    register_irq_handler(11, wakeup_irq_handler);
    register_irq_handler(12, wakeup_irq_handler);
    register_irq_handler(13, wakeup_irq_handler);
    register_irq_handler(14, wakeup_irq_handler);
    register_irq_handler(15, wakeup_irq_handler); //中断处理器
    
    asmv ("movq $0x125000, %rsp"); //设置堆
    enable_irq(0); //开启时钟中断
    enable_irq(1);
    enable_irq(2);
    enable_irq(3);
    enable_irq(4);
    enable_irq(5);
    enable_irq(6);
    enable_irq(7);
    enable_irq(8);
    enable_irq(9);
    enable_irq(10);
    enable_irq(11);
    enable_irq(12);
    enable_irq(13);
    enable_irq(14);
    enable_irq(15);
    asmv ("jmp init"); //跳转至INIT进程
    
    while(true);
}