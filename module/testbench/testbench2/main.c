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
#include <fifo.h>

#include <tayhuang/services.h>

#include <syscall/ipc.h>
#include <syscall/rpc.h>
#include <syscall/syscall.h>

#include <memory/sharemem.h>
#include <memory/malloc.h>

#include <export/__video_driver_fn.h>

PUBLIC void normal_ipc_handler(int caller, void *msg) {
    set_allow(ANY_TASK);
}

PUBLIC void main(void *args) {
    exit_thread(NULL);
}

PUBLIC void kmod_main(void) {
    set_logging_name("Testbench2");

    linfo ("Hi!I'm testbench2!");
    
    register_normal_ipc_handler(normal_ipc_handler);
    set_allow(ANY_TASK);

    create_thread(main, NULL);

    message_loop();

    while (true);
}