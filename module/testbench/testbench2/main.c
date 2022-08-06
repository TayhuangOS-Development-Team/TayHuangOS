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

PUBLIC void kmod_init(void) {
    set_logging_name("Testbench2");

    linfo ("Hi!I'm testbench2!");
    
    register_normal_ipc_handler(normal_ipc_handler);
    set_allow(ANY_TASK);
}

PUBLIC void kmod_main(void) {
    write_string(0, 0, 0x0A, "I'm testbench2");

    void *framebuffer = create_framebuffer(4, 4, 4, 4);
    linfo ("%p", framebuffer);
    *(byte *)(framebuffer + 0) = 'A';
    *(byte *)(framebuffer + 1) = 0x0A;
    *(byte *)(framebuffer + 2) = 'B';
    *(byte *)(framebuffer + 3) = 0x0A;
    *(byte *)(framebuffer + 4) = 'C';
    *(byte *)(framebuffer + 5) = 0x0A;
    swap_framebuffer(false);

    void *fifo = create_fifo(8000);
    share_fifo(fifo, 2);
    
    linfo ("%p", fifo);

    void *new_fifo = share_fifo(fifo, 2);
    linfo ("%p", new_fifo);

    send_msg((msgno_id){.message_no = MSG_NORMAL_IPC, .msg_id = get_msgid()}, (byte *)&new_fifo, sizeof(void *), 2);

    qword data = 114514;
    fifo_write_bytes(fifo, (byte *)&data, sizeof(qword));
}