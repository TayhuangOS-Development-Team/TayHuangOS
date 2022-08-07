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

PRIVATE bool start_with(const char *str1, const char *str2) {
    if (strlen(str1) < strlen(str2)) {
        return false;
    }
    while (*str2 != '\0') {
        if (*str1 != *str2) {
            return false;
        }
        str1 ++;
        str2 ++;
    }
    return true;
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

    tty_puts(0, "[CONSOLE 1]\n");
    tty_putchar(0, '>');

    char input[128] = {};
    int pos = 0;

    while (true) {
        char ch = tty_getchar(0);

        if (ch == '\n') {
            input[pos] = '\0';

            if (start_with(input, "echo ")) {
                if (input[5] == '\0') {
                    tty_puts(0, "Wrong arguments!\n");
                }
                else {
                    tty_puts(0, &input[5]);
                    tty_putchar(0, '\n');
                }
            }
            else {
                tty_puts(0, "Unknown Command!\n");
            }

            pos = 0;
            tty_putchar(0, '>');
        }
        else {
            input[pos] = ch;
            pos ++;
        }
    }
}