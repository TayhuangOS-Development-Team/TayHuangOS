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

void kmod_main(void) {
    set_print_color (0x0F);
    set_scroll_line (20);

    init_heap();

    int *x = malloc(sizeof(int));
    int *y = malloc(sizeof(int));

    printf ("x: %P ; y: %P ; ", x, y);
    free (x);

    int *z = malloc(sizeof(int));
    printf ("z: %P", z);
    while (true);
}