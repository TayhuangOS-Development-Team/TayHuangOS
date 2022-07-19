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
 * main.c
 *
 * testbench1主函数
 *
 */



#include <debug/logging.h>

#include <assert.h>

#include <memory/malloc.h>

#include <tayhuang/services.h>

#include <syscall/ipc.h>
#include <syscall/rpc.h>

PUBLIC void normal_ipc_handler(int caller, void *msg) {
    set_allow(ANY_TASK);
}

PUBLIC void kmod_main(void) {
    set_logging_name("Testbench2");

    linfo ("Hi!I'm testbench2!");
    
    register_normal_ipc_handler(normal_ipc_handler);
    set_allow(ANY_TASK);

    void *buffer = malloc(256);
    void *buf = buffer;

    ARG_WRITE(buf, int, 1);

    void *addr = remote_call(void*, SYSTASK_SERVICE, 1, ((rpc_args_struct){.data = buffer, .size = sizeof(int)}));
    *(qword*)(addr) = 8;

    buf = buffer;
    ARG_WRITE(buf, void*, addr);
    ARG_WRITE(buf, int, 1);
    ARG_WRITE(buf, int, 2);
    void *new_addr = remote_call(void*, SYSTASK_SERVICE, 0, ((rpc_args_struct){.data = buffer, .size = sizeof(void*) + sizeof(int) * 2}));

    linfo ("%p", new_addr);

    send_msg(MSG_NORMAL_IPC, &new_addr, sizeof(void*), 2);

    free(buffer);

    message_loop();

    while (true);
}