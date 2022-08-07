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

#include <export/video/__video_driver_fn.h>
#include <export/keyboard/__keyboard_driver_fn.h>
#include <export/tty/__tty_driver_fn.h>
#include <string.h>

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
    void *fifo = create_fifo(8000);
    share_fifo(fifo, 2);
    
    linfo ("%p", fifo);

    void *new_fifo = share_fifo(fifo, 2);
    linfo ("%p", new_fifo);

    send_msg((msgno_id){.message_no = MSG_NORMAL_IPC, .msg_id = get_msgid()}, (byte *)&new_fifo, sizeof(void *), 2);

    qword data = 114514;
    fifo_write_bytes(fifo, (byte *)&data, sizeof(qword));

    void *buffer = get_console(0);
    *(byte*)(buffer + 0x00) = '['; *(byte*)(buffer + 0x01) = 0x0A;
    *(byte*)(buffer + 0x02) = 'C'; *(byte*)(buffer + 0x03) = 0x0A;
    *(byte*)(buffer + 0x04) = 'O'; *(byte*)(buffer + 0x05) = 0x0A;
    *(byte*)(buffer + 0x06) = 'N'; *(byte*)(buffer + 0x07) = 0x0A;
    *(byte*)(buffer + 0x08) = 'S'; *(byte*)(buffer + 0x09) = 0x0A;
    *(byte*)(buffer + 0x0A) = 'O'; *(byte*)(buffer + 0x0B) = 0x0A;
    *(byte*)(buffer + 0x0C) = 'L'; *(byte*)(buffer + 0x0D) = 0x0A;
    *(byte*)(buffer + 0x0E) = 'E'; *(byte*)(buffer + 0x0F) = 0x0A;
    *(byte*)(buffer + 0x10) = ' '; *(byte*)(buffer + 0x11) = 0x0A;
    *(byte*)(buffer + 0x12) = '1'; *(byte*)(buffer + 0x13) = 0x0A;
    *(byte*)(buffer + 0x14) = ']'; *(byte*)(buffer + 0x15) = 0x0A;
    flush_console(0);

    int x = 0, y = 1;
    while (true) {
        key_t key = getkey();
        write_char(x, y, 0x0A, key);
        x ++;
    }
}