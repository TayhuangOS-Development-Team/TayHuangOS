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
 * init.c
 * 
 * 初始化
 * 
 */



#include <tayhuang/defs.h>
#include <tayhuang/kmod_prototypes.h>
#include <tayhuang/services.h>
#include <tayhuang/msgpack.h>

#include <debug/logging.h>

#include <assert.h>

#include <memory/malloc.h>

#include <syscall/syscall.h>
#include <syscall/ipc.h>

#define MAIL_SIZE (8 * 1024)

PRIVATE void __message_loop(void *args) {
    //分配邮箱缓冲区
    void *mail_buffer = malloc(MAIL_SIZE);
    set_mailbuffer(mail_buffer, MAIL_SIZE);

    set_allow(ANY_TASK);

    //通知init该模块已初始化完成
    bool status = true;
    bool send_ret = send_msg((msgno_id){.message_no = MSG_NORMAL_IPC, .msg_id = get_msgid()}, &status, sizeof(bool), INIT_SERVICE);
    assert(send_ret);

    message_loop();
    exit_thread(NULL);
}

PUBLIC void __kmod_init__(void) {
    register int magic __asm__("rax");
    register void *heap_start __asm__("rbx");
    register void *heap_end __asm__("rcx");
    register int pid __asm__("rdx");

    if (magic != KMOD_MAGIC) {
        while (true);
    }

    //设置日志名
    set_logging_name("KMod Init");
    //初始化堆
    init_heap(pid, heap_start, heap_end);

    self_pid = pid;

    kmod_init();

    create_thread(__message_loop, NULL);

    kmod_main();

    id_t signal = create_signal(1, 0, false);
    down_signal(signal);

    while (true);
    //exit();
}
