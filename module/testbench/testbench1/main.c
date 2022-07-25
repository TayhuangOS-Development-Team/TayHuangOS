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

#include <fifo.h>

PUBLIC void normal_ipc_handler(int caller, void *msg) {
    set_allow(ANY_TASK);

    void *fifo = *(void **)msg;
    linfo ("%p", fifo);
    qword data;

    while (! fifo_read_bytes(fifo, (byte*)&data, sizeof(qword)));
    
    linfo ("%d", data);
}

PUBLIC void kmod_main(void) {
    set_logging_name("Testbench1");

    linfo ("Hi!I'm testbench1!");
    
    register_normal_ipc_handler(normal_ipc_handler);
    set_allow(ANY_TASK);

    message_loop();

    while (true);
}