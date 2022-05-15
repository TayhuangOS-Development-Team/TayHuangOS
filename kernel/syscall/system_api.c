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

PUBLIC void after_syscall(struct intterup_args *regs);

PRIVATE volatile int ticks = 0;

PUBLIC void video_api_process(void) {
    while (true) {
        qword pack[20];
        while (receive_any_msg(pack) == -1);
        asmv ("xchg %bx, %bx");
        if (pack[0] == 0) { //puts
            puts(pack[1]);
        }
        else if (pack[0] == 1) { //putchar
            putchar ((char)pack[1]);
        }
        else if (pack[0] == 2) { //printint
            printk ("%d", pack[1]);
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
        if (pack[0] == 0) { //get_ticks
            int tks = ticks;
            send_msg(&tks, caller, sizeof(int));
        }
    }
}