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

PUBLIC void __kmod_init__(void) {
    register int magic __asm__("rax");
    register void *heap_start __asm__("rbx");
    register void *heap_end __asm__("rcx");
    register int pid __asm__("rdx");

    if (magic != KMOD_MAGIC) {
        while (true);
    }

    set_logging_name("KMod Init");
    init_heap(pid, heap_start, heap_end);

    void *mail_buffer = malloc(MAIL_SIZE);
    set_mailbuffer(mail_buffer, MAIL_SIZE);

    set_allow(ANY_TASK);

    bool status = true;
    assert(send_msg(MSG_NORMAL_IPC, status, sizeof(bool), INIT_SERVICE));

    kmod_main();

    while (true);
    //exit();
}