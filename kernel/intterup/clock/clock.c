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
 * clock.c
 * 
 * 时钟驱动
 * 
 */



#include "clock.h"

#include <syscall/syscall.h>
#include <process/task_manager.h>

#include <string.h>

PUBLIC void after_syscall(struct intterup_args *regs);

PUBLIC volatile int ticks = 0;

PUBLIC short clock_int_handler(int irq, struct intterup_args *regs, bool entered_handler) { //时钟中断
    ticks ++;

    if (! entered_handler)
        after_syscall(regs);

    current_task->counter --;

    return 0;
}