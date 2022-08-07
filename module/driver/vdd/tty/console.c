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
 * console.c
 * 
 * 控制台
 * 
 */



#include <console.h>
#include <export/video/__video_driver_fn.h>
#include <export/tty/__tty_driver_fn.h>
#include <memory/sharemem.h>
#include <memory/malloc.h>
#include <syscall/rpc.h>

PUBLIC console_t consoles[CONSOLE_NUM];
PUBLIC int current_active_console;

PUBLIC void init_consoles(void) {
    current_active_console = 0;
    for (int i = 0 ; i < CONSOLE_NUM ; i ++) {
        create_framebuffer_result ret = create_framebuffer(0, 0, CONSOLE_COLUMNS, CONSOLE_LINES);
        consoles[i].framebuffer = ret.framebuffer;
        consoles[i].framebuffer_id = ret.id;
        consoles[i].pos_x = 0;
        consoles[i].pos_y = 0;
        consoles[i].color = 0x0F;
    }
}

PRIVATE void __flush_console(int console_no) {
    swap_framebuffer(consoles[console_no].framebuffer_id);
}


PRIVATE void __putchar(int console_no, char ch) {
    int pos = consoles[console_no].pos_y * CONSOLE_COLUMNS + consoles[console_no].pos_x;
    *(byte *)(consoles[console_no].framebuffer + pos * 2) = ch;
    *(byte *)(consoles[console_no].framebuffer + pos * 2 + 1) = consoles[console_no].color;
    consoles[console_no].pos_x ++;
    if (consoles[console_no].pos_x == CONSOLE_COLUMNS) {
        consoles[console_no].pos_x = 0;
        consoles[console_no].pos_y ++;
    }
}

PRIVATE rpc_args_struct wrapper_flush_console(int caller, rpc_func func_no, rpc_args_struct args) {
    int console_no = ARG_READ(args.data, int);

    __flush_console(console_no);

    bool *result = (bool *)malloc(sizeof(bool));
    *result = true;

    return (rpc_args_struct){.data = result, .size = sizeof(FLUSH_CONSOLE_RETURN_TYPE)};
}

PUBLIC void console_register_rpc_functions(void) {
    rpc_register(FLUSH_CONSOLE_FN, wrapper_flush_console, sizeof(FLUSH_CONSOLE_RETURN_TYPE), FLUSH_CONSOLE_ARGS_SIZE);
}

PUBLIC void echo_ch(char ch) {
    __putchar(current_active_console, ch);
    __flush_console(current_active_console);
}