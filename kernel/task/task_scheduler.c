/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 * 
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 * 
 * --------------------------------------------------------------------------------
 * 
 * 作者: theflysong
 * 
 * task_scheduler.h
 * 
 * 进程调度器
 * 
 */



#include <task/task_scheduler.h>
#include <task/task_manager.h>

#include <string.h>

#include <logging.h>

PUBLIC task_struct *current_task;
PUBLIC bool do_schedule = false;

PUBLIC void start_schedule(void) {
    do_schedule = true;
}

PUBLIC task_struct *get_next_run_task(void) {
    if (has_level0_task()) {
        return dequeue_level0_task();
    }
    return dequeue_level1_task();
}

PUBLIC bool should_switch(void) {
    if (! do_schedule) {
        return false;
    }
    if (current_task->level == 0) {
        if (current_task->state == RUNNING) {
            return false;
        }
        return true;
    }
    else if (current_task->level == 1) {
        if (has_level0_task()) {
            return true;
        }
        if (current_task->state != RUNNING) {
            return true;
        }
        if (current_task->count <= 0) {
            return true;
        }
        return false;
    }
    return true;
}

PRIVATE void down_task(struct intterup_args *regs) {
    memcpy(&current_task->thread_info.basic, regs, sizeof(current_task->thread_info.basic));

    current_task->thread_info.stack.rbp      = regs->rbp;
    current_task->thread_info.stack.rsp      = regs->rsp;
    current_task->thread_info.stack.ss       = regs->ss;

    current_task->thread_info.program.cs     = regs->cs;
    current_task->thread_info.program.rip    = regs->rip;
    current_task->thread_info.program.rflags = regs->rflags;

    if (current_task->state == RUNNING) {
        current_task->state = READY;

        if (current_task->level == 0) {
            enqueue_level0_task(current_task);
        }
        else if (current_task->level == 1) {
            enqueue_level1_task(current_task);
        }
    }
}

PRIVATE void up_task(struct intterup_args *regs) {
    memcpy(regs, &current_task->thread_info.basic, sizeof(current_task->thread_info.basic));
 
    regs->rbp    = current_task->thread_info.stack.rbp;
    regs->rsp    = current_task->thread_info.stack.rsp;
    regs->ss     = current_task->thread_info.stack.ss;

    regs->cs     = current_task->thread_info.program.cs;
    regs->rip    = current_task->thread_info.program.rip;
    regs->rflags = current_task->thread_info.program.rflags;

    regs->pgd    = current_task->mm_info.pgd;

    current_task->state = RUNNING;
}

PUBLIC void do_switch(struct intterup_args *regs) {
    down_task(regs);

    current_task = get_next_run_task();

    up_task(regs);

    if (current_task->level == 1) {
        current_task->count = current_task->priority * 3;
    }
}

PUBLIC void after_syscall(struct intterup_args *regs) {
    if (should_switch()) {
        do_switch(regs);
    }
}