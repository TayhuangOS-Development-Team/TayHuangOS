/*
 * SPDX-License-Identifier: LGPL-2.1-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: theflysong
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
#include <tayhuang/video_info.h>

#include <string.h>
#include <assert.h>

#include <memory/kheap.h>
#include <memory/pmm.h>
#include <memory/paging.h>
#include <memory/shared_memory.h>

#include <intterup/init_int.h>
#include <intterup/irq_handler.h>
#include <intterup/clock.h>

#include <task/task_manager.h>
#include <task/task_scheduler.h>
#include <task/signal.h>

#include <syscall/syscall.h>
#include <syscall/rpc.h>
#include <syscall/syscalls.h>
#include <syscall/sys_task.h>

#include <printk.h>
#include <logging.h>
#include <global.h>
#include <kmod_loader.h>

PUBLIC void *kernel_pml4 = NULL;
PUBLIC struct boot_args args;
PUBLIC int cur_pid = 2;
PUBLIC word msgid_counter = 0;

PRIVATE struct desc_struct GDT[16];
PRIVATE struct gdt_ptr gdtr;
PRIVATE struct tss TSS;

PRIVATE inline void set_tss(void *addr, qword base, dword limit, byte type) { //设置TSS
    struct tss_struct *desc = (struct tss_struct *)addr;
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

#define CLOCK_FREQUENCY (50.0f) //时钟周期

#define SETUP_MOD_BASE (64 * 1024)
#define SETUP_MOD_SIZE (320 * 1024)
#define KHEAP_PRE_BASE (SETUP_MOD_BASE + SETUP_MOD_SIZE)
#define KHEAP_PRE_SIZE (640 * 1024 - KHEAP_PRE_BASE)

//0 ~ 32MB系统保留
#define RESERVED_LIMIT (0x2000000)

program_info load_mod_by_setup(const char *name) {
    #define MOD_SIZE (64 * 1024)
    void *mod_addr = kmalloc(MOD_SIZE);

    bool send_ret = send_msg((msgno_id){.message_no = MSG_NORMAL_IPC, .msg_id = get_msgid()}, (void *)name, strlen(name) + 1, SETUP_SERVICE);
    assert(send_ret);

    send_ret = send_msg((msgno_id){.message_no = MSG_NORMAL_IPC, .msg_id = get_msgid()}, &mod_addr, sizeof(mod_addr), SETUP_SERVICE);
    assert(send_ret);

    set_mapping(get_task_by_pid(SETUP_SERVICE)->mm_info.pgd, mod_addr, mod_addr, MOD_SIZE / MEMUNIT_SZ, true, false);

    bool status;

    set_allow(SETUP_SERVICE);
    check_ipc();
    recv_msg(&status);

    if (! status) {
        lerror ("Init", "Failed to load %s!", name);
        while (1);
    }

    program_info mod_info = load_kmod_from_memory(mod_addr);

    kfree(mod_addr);

    return mod_info;
}

PUBLIC program_info load_and_init_mod(const char *modname, const char *name, int pid, int level) {
    program_info mod_info = load_mod_by_setup(modname);
    initialize_kmod_task(
        create_task(DS_SERVICE, mod_info.stack_top, mod_info.stack_bottom, mod_info.entry, CS_SERVICE, RFLAGS_SERVICE,
                    mod_info.pgd, mod_info.start, mod_info.end, mod_info.start, mod_info.end,
                     mod_info.heap_bottom, mod_info.heap_top,mod_info.start, mod_info.end,
                     DEFAULT_SHM_START, DEFAULT_SHM_END,
                    pid, 1, level, current_thread->task, false));

    bool status;
    set_allow(pid);
    check_ipc();
    recv_msg(&status);
    if (! status) {
        lerror ("Init", "Failed to initialize %s!", name);
        while (1);
    }

    return mod_info;
}

//first task-init
PUBLIC void init(void) {
    void *mail = kmalloc(8192);
    set_mailbuffer(mail, 8192);
    
    linfo ("Init", "Hi!I'm Init!");

    //do something
    start_schedule();

    //-------------------SYS TASK------------------------

    create_task(DS_SERVICE, RING0_STACKTOP2, RING0_STACKBOTTOM2, sys_task, CS_SERVICE, RFLAGS_SERVICE,
                kernel_pml4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                SYSTASK_SERVICE, 1, 0, current_thread->task, true);
    bool status;
    
    set_allow(SYSTASK_SERVICE);
    check_ipc();
    recv_msg(&status);
    if (! status) {
        lerror ("Init", "Failed to initialize sys_task()!");
        while (1);
    }

    //---------------------SETUP-------------------------
    program_info setup_mod_info = load_kmod_from_memory(SETUP_MOD_BASE);
    
    initialize_kmod_task(
        create_task(DS_SERVICE, setup_mod_info.stack_top, setup_mod_info.stack_bottom, setup_mod_info.entry, CS_SERVICE, RFLAGS_SERVICE,
                    setup_mod_info.pgd, setup_mod_info.start, setup_mod_info.end, setup_mod_info.start, setup_mod_info.end,
                     setup_mod_info.heap_bottom, setup_mod_info.heap_top,setup_mod_info.start, setup_mod_info.end,
                     DEFAULT_SHM_START, DEFAULT_SHM_END,
                    SETUP_SERVICE, 1, 0, current_thread->task, false));

    set_allow(SETUP_SERVICE);
    check_ipc();
    recv_msg(&status);
    if (! status) {
        lerror ("Init", "Failed to initialize setup module!");
        while (1);
    }

    //---------------------VIDEO-------------------------
    
    program_info video_mod_info = load_and_init_mod("video.mod", "video driver", VIDEO_DRIVER_SERVICE, 0);

    //TODO: 更改loader以获取framebuffer的bpp
    set_mapping(video_mod_info.pgd, args.framebuffer, args.framebuffer, (args.is_graphic_mode ? 0x6000000 : 0x8000) / MEMUNIT_SZ, true, false);

    video_info_struct video_info;
    video_info.framebuffer = args.framebuffer;
    video_info.height = args.screen_height;
    video_info.width = args.screen_width;
    video_info.is_graphic_mode = args.is_graphic_mode;
    send_msg((msgno_id){.message_no = MSG_NORMAL_IPC, .msg_id = get_msgid()}, &video_info, sizeof(video_info_struct), VIDEO_DRIVER_SERVICE);
    
    //-------------------KEYBOARD-----------------------
    load_and_init_mod("keyboard.man", "keyboard I/O manager", KEYBOARD_IOMAN_SERVICE, 0);
    load_and_init_mod("keyboard.mod", "keyboard driver", KEYBOARD_DRIVER_SERVICE, 0);

    //---------------------TTY---------------------------
    load_and_init_mod("tty.mod", "TTY", TTY_DRIVER_SERVICE, 0);

    //-------------------TESTBENCH-----------------------
    load_and_init_mod("tb1.mod", "testbench 1", alloc_pid(), 1);
    load_and_init_mod("tb2.mod", "testbench 2", alloc_pid(), 1);

    check_ipc();

    linfo ("Init", "Init End");
    
    while (true);
}

PUBLIC void mapping_kernel(void *pgd) {
    set_mapping(pgd, NULL, NULL, RESERVED_LIMIT / MEMUNIT_SZ, true, false);
}

//初始化
PUBLIC void initialize(void) {
    //初始化GDT
    init_gdt();
    
    //初始化输出
    init_serial();

    init_video(args.framebuffer, args.screen_width, args.screen_height);

    //初始化中断
    init_idt();
    init_pic();

    init_sse();
    init_pit(CLOCK_FREQUENCY);

    //复制setup mod
    memcpy (SETUP_MOD_BASE, args.setup_mod_addr, SETUP_MOD_SIZE);

    //初始化内存
    init_kheap(KHEAP_PRE_BASE, KHEAP_PRE_SIZE);

    qword memsz = args.memory_size;

    init_pmm(memsz, 0x2000000);

    kernel_pml4 = create_pgd();

    set_mapping(kernel_pml4, NULL, NULL, memsz / MEMUNIT_SZ, true, true);
    mapping_kernel(kernel_pml4);
    
    if (((qword)args.framebuffer) >= memsz) {
        set_mapping(kernel_pml4, args.framebuffer, args.framebuffer, (args.screen_width * args.screen_height * 3 * 2) / MEMUNIT_SZ, true, true);
    }

    __set_cr3(kernel_pml4);

    create_empty_task(); //创建空进程
    cur_pid = 2;

    //初始化TSS
    TSS.ist1 = IST0_STACKTOP;
    TSS.rsp0 = RING0_STACKTOP;

    //初始化中断
    register_irq_handler(0, clock_int_handler);

    for (int i = 1 ; i < 16 ; i ++) {
        register_irq_handler(i, normal_irq_handler);
    }

    for (int i = 0 ; i < 16 ; i ++) {
        enable_irq(i);
    }

    en_int();
}

PUBLIC void entry(struct boot_args *_args) {
    //Boot Arg
    if (_args->magic != BOOT_ARGS_MAGIC) {
        while (true);
    }
    memcpy(&args, _args, sizeof(struct boot_args));

    initialize(); //初始化

    linfo ("Kernel", "Hello, I'm TayhuangOS Kernel!");

    current_thread = __create_task(DS_SERVICE, RING0_STACKTOP, RING0_T1STACKBOTTOM, init, CS_SERVICE, RFLAGS_SERVICE,
                 kernel_pml4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                 INIT_SERVICE, 1, 0, NULL, true)->threads;

    add_task(current_thread->task);
    current_thread->state = RUNNING;

    asmv ("movq %0, %%rsp" : : "g"(RING0_STACKTOP));
    asmv ("jmp init");

    while (true);
}
