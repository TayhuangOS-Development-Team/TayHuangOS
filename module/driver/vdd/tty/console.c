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

PUBLIC void init_consoles(void) {
    for (int i = 0 ; i < CONSOLE_NUM ; i ++) {
        create_framebuffer_result ret = create_framebuffer(0, 0, CONSOLE_COLUMNS, CONSOLE_LINES);
        consoles[i].framebuffer = ret.framebuffer;
        consoles[i].framebuffer_id = ret.id;
    }
}

PRIVATE void *__get_console(int console_no, int caller) {
    if (console_no < 0 || console_no > CONSOLE_NUM) {
        return NULL;
    }

    void *addr = share_memory(consoles[console_no].framebuffer, CONSOLE_PAGES, caller);
    return addr;
}

PRIVATE rpc_args_struct wrapper_get_console(int caller, rpc_func func_no, rpc_args_struct args) {
    int console_no = ARG_READ(args.data, int);

    void **result = (void **)malloc(sizeof(void *));
    *result = __get_console(console_no, caller);

    return (rpc_args_struct){.data = result, .size = sizeof(GET_CONSOLE_RETURN_TYPE)};
}

PRIVATE void __flush_console(int console_no) {
    swap_framebuffer(consoles[console_no].framebuffer_id);
}

PRIVATE rpc_args_struct wrapper_flush_console(int caller, rpc_func func_no, rpc_args_struct args) {
    int console_no = ARG_READ(args.data, int);

    __flush_console(console_no);

    bool *result = (bool *)malloc(sizeof(bool));
    *result = true;

    return (rpc_args_struct){.data = result, .size = sizeof(FLUSH_CONSOLE_RETURN_TYPE)};
}

PUBLIC void console_register_rpc_functions(void) {
    rpc_register(GET_CONSOLE_FN, wrapper_get_console, sizeof(GET_CONSOLE_RETURN_TYPE), GET_CONSOLE_ARGS_SIZE);
    rpc_register(FLUSH_CONSOLE_FN, wrapper_flush_console, sizeof(FLUSH_CONSOLE_RETURN_TYPE), FLUSH_CONSOLE_ARGS_SIZE);
}