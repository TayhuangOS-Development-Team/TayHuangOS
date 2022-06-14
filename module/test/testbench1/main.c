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



#include <debug/logging.h>
#include <printf.h>
#include <memory/malloc.h>

extern int pos_x;
extern int pos_y;
extern int current_pid;

void kmod_main(void) {
    set_logging_name("TESTBENCH1");
    
    set_print_color (0x0F);
    set_scroll_line (20);

    set_tty(0);

    init_heap();

    int *x = malloc(sizeof(int));
    int *y = malloc(sizeof(int));

    printf ("x: %P ; y: %P ; ", x, y);
    free (x);

    int *z = malloc(sizeof(int));
    printf ("z: %P", z);

    free(z);
    free(y);

    clrscr();

    printf ("Hello, World!");

    set_tty(1);

    printf ("Hi!");

    set_active_tty(1);

    while (true);
}