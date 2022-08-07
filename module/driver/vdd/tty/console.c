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
#include <syscall/syscall.h>

PUBLIC console_t consoles[CONSOLE_NUM];
PUBLIC int current_active_console;
PUBLIC id_t wait_enter;

PUBLIC void init_consoles(void) {
    wait_enter = create_signal(CONSOLE_LINES, 0, false);

    current_active_console = 0;
    for (int i = 0 ; i < CONSOLE_NUM ; i ++) { //初始化console
        create_framebuffer_result ret = create_framebuffer(0, 0, CONSOLE_COLUMNS, CONSOLE_LINES);
        consoles[i].framebuffer = ret.framebuffer;
        consoles[i].framebuffer_id = ret.id;
        consoles[i].pos_x = 0;
        consoles[i].pos_y = 0;
        consoles[i].color = 0x0F;
        consoles[i].console_no = i;
    }
}

PRIVATE void __flush_console(int console_no) {
    if (console_no == current_active_console) { //是当前活跃的console
        swap_framebuffer(consoles[console_no].framebuffer_id);
    }
}

PRIVATE rpc_args_struct wrapper_flush_console(int caller, rpc_func func_no, rpc_args_struct args) {
    int console_no = ARG_READ(args.data, int);

    __flush_console(console_no);

    bool *result = (bool *)malloc(sizeof(bool));
    *result = true;

    return (rpc_args_struct){.data = result, .size = sizeof(FLUSH_CONSOLE_RETURN_TYPE)};
}

PRIVATE void __putrawchar(int console_no, char ch) {
    int pos = consoles[console_no].pos_y * CONSOLE_COLUMNS + consoles[console_no].pos_x;
    *(byte *)(consoles[console_no].framebuffer + pos * 2) = ch;
    *(byte *)(consoles[console_no].framebuffer + pos * 2 + 1) = consoles[console_no].color;
    consoles[console_no].pos_x ++;
    if (consoles[console_no].pos_x == CONSOLE_COLUMNS) {
        consoles[console_no].pos_x = 0;
        consoles[console_no].pos_y ++;
    }
}

PRIVATE void __putchar(int console_no, char ch) {
    if (ch == '\r' || ch == '\n') { //换行
        consoles[console_no].pos_x = 0;
        consoles[console_no].pos_y ++;
    }
    else if (ch == '\t') { //缩进
        consoles[console_no].pos_x += 4;
    }
    else if (ch == '\v') { //下一行(x不变)
        consoles[console_no].pos_y ++;
    }
    else if (ch == '\b') { //退格
        consoles[console_no].pos_x --; //回到上一个pos
        if (consoles[console_no].pos_x < 0) {
            consoles[console_no].pos_x = CONSOLE_COLUMNS - 1;
            consoles[console_no].pos_y --;
        }
        if (consoles[console_no].pos_y < 0) {
            consoles[console_no].pos_x = consoles[console_no].pos_y = 0;
        }

        __putrawchar(console_no, ' '); //用' '填充

        consoles[console_no].pos_x --; //回到上一个pos
        if (consoles[console_no].pos_x < 0) {
            consoles[console_no].pos_x = CONSOLE_COLUMNS - 1;
            consoles[console_no].pos_y --;
        }
    }
    else if(ch == '\f') { //换页
        //clrscr();
        consoles[console_no].pos_x = 0;
        consoles[console_no].pos_y = 0;
    }
    else {
        __putrawchar(console_no, ch);
    }
}

PRIVATE rpc_args_struct wrapper_putchar(int caller, rpc_func func_no, rpc_args_struct args) {
    int console_no = ARG_READ(args.data, int);
    char ch = ARG_READ(args.data, char);

    __putchar(console_no, ch); //输出
    __flush_console(console_no); //刷新

    bool *result = (bool *)malloc(sizeof(bool));
    *result = true;

    return (rpc_args_struct){.data = result, .size = sizeof(FLUSH_CONSOLE_RETURN_TYPE)};
}

PRIVATE rpc_args_struct wrapper_puts(int caller, rpc_func func_no, rpc_args_struct args) {
    bool *result = (bool *)malloc(sizeof(bool));

    if (args.size < PUTS_ARGS_BASE_SIZE) { //不足基本大小
        *result = false;
        return (rpc_args_struct){.data = result, .size = sizeof(PUTS_RETURN_TYPE)};
    }

    int console_no = ARG_READ(args.data, int);
    int num = ARG_READ(args.data, int);

    if (args.size != (PUTS_ARGS_BASE_SIZE + num)) { //大小不匹配
        *result = false;
        return (rpc_args_struct){.data = result, .size = sizeof(PUTS_RETURN_TYPE)};
    }

    for (int i = 0 ; i < num ; i ++) {
        char ch = ARG_READ(args.data, char);
        __putchar(console_no, ch); //输出
    }

    __flush_console(console_no); //刷新

    *result = true;

    return (rpc_args_struct){.data = result, .size = sizeof(PUTS_RETURN_TYPE)};
}

PRIVATE char escape_key(key_t key) {
    if (key == ENTER) { //转换成'\n'
        key = '\n';
    }
    else if (key == BACKSPACE) { //转换成'\b'
        key = '\b';
    }
    else if (key == TAB) { //转换成'\t'
        key = '\t';
    }
    return key;
}

PRIVATE char __getchar(void) {
    down_signal(wait_enter); //等待回车

    key_t key = getkey();

    char ch = escape_key(key);
    
    if (key != ENTER) { //不为回车
        up_signal(wait_enter); //回车数量不减少
    }

    return ch;
}

PRIVATE rpc_args_struct wrapper_getchar(int caller, rpc_func func_no, rpc_args_struct args) {
    int console_no = ARG_READ(args.data, int);

    char *result = (char *)malloc(sizeof(char));

    if (console_no == current_active_console) { //是当前活跃的控制台
        *result = __getchar();
    }
    else {
        *result = '\0';
    }

    return (rpc_args_struct){.data = result, .size = sizeof(FLUSH_CONSOLE_RETURN_TYPE)};
}

PUBLIC void console_register_rpc_functions(void) {
    rpc_register(FLUSH_CONSOLE_FN, wrapper_flush_console, sizeof(FLUSH_CONSOLE_RETURN_TYPE), FLUSH_CONSOLE_ARGS_SIZE);
    rpc_register(PUTCHAR_FN, wrapper_putchar, sizeof(PUTCHAR_RETURN_TYPE), PUTCHAR_ARGS_SIZE);
    rpc_register(PUTS_FN, wrapper_puts, sizeof(PUTS_RETURN_TYPE), PUTS_ARGS_SIZE);
    rpc_register(GETCHAR_FN, wrapper_getchar, sizeof(GETCHAR_RETURN_TYPE), GETCHAR_ARGS_SIZE);
}

//更改: 先发送到缓冲区中 等到enter出现再移动显示指针(注:发送到缓冲区时显示 但不移动显示指针)
PUBLIC void echo_key(key_t key) {
    key = escape_key(key);
    __putchar(current_active_console, key);
    __flush_console(current_active_console);
}
