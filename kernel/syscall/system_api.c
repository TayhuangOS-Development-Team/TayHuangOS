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
 * system_api.h
 * 
 * 系统api
 * 
 */



#include "system_api.h"
#include "syscall.h"
#include "../process/task_manager.h"
#include "../display/printk.h"

#include <string.h>

PUBLIC void after_syscall(struct intterup_args *regs);

PUBLIC volatile int ticks = 0;

PUBLIC void video_api_process(void) {
    while (true) {
        qword pack[20];
        int caller = 0;
        while ((caller = receive_any_msg(pack)) == -1);
        if (pack[0] == 0) { //puts
            dis_int();
            int oldx = get_pos_x(), oldy = get_pos_y();
            int oldcolor = get_print_color();

            change_pos(pack[2], pack[3]);
            set_print_color(pack[4]);

            puts(pack[1]);

            set_print_color(oldcolor);
            change_pos(oldx, oldy);
            en_int();

            int len = strlen(pack[1]);
            send_msg(&len, caller, sizeof(int), 20);
        }
        else if (pack[0] == 1) { //putchar
            dis_int();
            int oldx = get_pos_x(), oldy = get_pos_y();
            int oldcolor = get_print_color();

            change_pos(pack[2], pack[3]);
            set_print_color(pack[4]);

            putchar ((char)pack[1]);

            set_print_color(oldcolor);
            change_pos(oldx, oldy);
            en_int();

            int len = 1;
            send_msg(&len, caller, sizeof(int), 20);
        }
        else if (pack[0] == 2) { //printint
            dis_int();
            int oldx = get_pos_x(), oldy = get_pos_y();
            int oldcolor = get_print_color();

            change_pos(pack[2], pack[3]);
            set_print_color(pack[4]);

            int len = printk ("%d", pack[1]);

            set_print_color(oldcolor);
            change_pos(oldx, oldy);
            en_int();

            send_msg(&len, caller, sizeof(int), 20);
        }
    }
}

PUBLIC short clock_int_handler(int irq, struct intterup_args *regs, bool entered_handler) { //时钟中断
    ticks ++;

    if (! entered_handler)
        after_syscall(regs);

    current_task->counter --;

    return 0;
}

PUBLIC void clock_api_process(void) {
    while (true) {
        qword pack[20];
        int caller = 0;
        while ((caller = receive_any_msg(pack)) == -1);
    }
}