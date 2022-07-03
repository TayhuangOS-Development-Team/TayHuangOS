/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * main.c
 *
 * testbench 主函数
 *
 */



#include <tayhuang/defs.h>

#include <debug/logging.h>
#include <printf.h>
#include <memory/malloc.h>

EXTERN PUBLIC int pos_x;
EXTERN PUBLIC int pos_y;
EXTERN PUBLIC int current_pid;

PUBLIC void kmod_main(void) {
    set_logging_name("Testbench1");
    
    set_print_color (0x0F);
    set_scroll_line (20);

    set_tty(0);

    init_heap();

    clrscr();

    for (int i = 0 ; i < 3 ; i ++) {
        set_tty(i);
        printf ("[TTY %d]\n", i);
    }

    set_tty(1);

    printf ("Hi!\nI'm TTY1!!!!!!!");

    set_active_tty(1);

    while (true);
}