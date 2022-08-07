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
 * tty主函数
 * 
 */



#include <tayhuang/defs.h>
#include <tayhuang/services.h>

#include <debug/logging.h>
#include <export/keyboard/__keyboard_driver_fn.h>

#include <syscall/ipc.h>
#include <syscall/syscall.h>

#include <console.h>

PRIVATE key_t key = 0;
PRIVATE id_t signal = 0;

PRIVATE void echo_input(void *args) {
    while (true) {
        //等待key
        down_signal(signal);
        echo_key(key);
    }

    exit_thread(NULL);
} 

PUBLIC void normal_ipc_handler(int caller, void *msg) {
    if (caller == KEYBOARD_DRIVER_SERVICE) {
        key = *(key_t*)msg;
        up_signal(signal); //唤醒回显进程
        if (key == ENTER) {
            up_signal(wait_enter); //唤醒等待回车的进程
        }
    }
}

PUBLIC void kmod_init(void) {
    set_logging_name("TTY");
    linfo ("Hi!I'm TTY!");

    console_register_rpc_functions();
    register_normal_ipc_handler(normal_ipc_handler);

    //用于唤醒回显进程
    signal = create_signal(1, 0, false);
    create_thread(echo_input, NULL);
}

PUBLIC void kmod_main(void) {
    init_consoles(); //TODO: 在init_consoles调用完前拒绝rpc请求
}