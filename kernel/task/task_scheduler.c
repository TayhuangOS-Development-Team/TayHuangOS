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
 * task_scheduler.h
 * 
 * 进程调度器
 * 
 */



#include <task/task_scheduler.h>
#include <task/task_manager.h>

#include <string.h>

#include <logging.h>

PUBLIC thread_info_struct *current_thread;
PUBLIC bool do_schedule = false;

//开始进行调度
PUBLIC void start_schedule(void) {
    do_schedule = true;
}

//获得下个运行的进程
PUBLIC thread_info_struct *get_next_run_thread(void) {
    //level0是否有进程
    if (has_level0_thread()) {
        return dequeue_level0_thread();
    }
    //level1是否有进程
    if (has_level1_thread()) {
        return dequeue_level1_thread();
    }
    //空进程
    return get_empty_task()->threads;
}

//是否需要调度
PUBLIC bool should_switch(void) {
    if (! do_schedule) { //还未开始调度
        return false;
    }
    if (current_thread->task->pid == -1) { //空进程
        return true;
    }
    if (current_thread->task->level == 0) { //level=0 FIFO算法
        if (current_thread->state == RUNNING) { //还在运行则不调度
            return false;
        }
        return true;
    }
    else if (current_thread->task->level == 1) { //level=1 TRR算法
        if (has_level0_thread()) { //level0有进程则调度
            return true;
        }
        if (current_thread->state != RUNNING) { //不在运行则调度
            return true;
        }
        if (current_thread->count <= 0) { //时间片用尽则调度
            return true;
        }
        return false;
    }
    return true;
}

//下降进程
PRIVATE void down_thread(struct intterup_args *regs) {
    memcpy(&current_thread->registers.basic, regs, sizeof(current_thread->registers.basic));

    current_thread->registers.stack.rbp      = regs->rbp;
    current_thread->registers.stack.rsp      = regs->rsp;
    current_thread->registers.stack.ss       = regs->ss;

    current_thread->registers.program.cs     = regs->cs;
    current_thread->registers.program.rip    = regs->rip;
    current_thread->registers.program.rflags = regs->rflags;

    if (current_thread->state == RUNNING) {
        current_thread->state = READY;

        if (current_thread->task->level == 0) {
            enqueue_level0_thread(current_thread);
        }
        else if (current_thread->task->level == 1) {
            enqueue_level0_thread(current_thread);
        }
    }
}

//上升进程
PRIVATE void up_thread(struct intterup_args *regs) {
    memcpy(regs, &current_thread->registers.basic, sizeof(current_thread->registers.basic));
 
    regs->rbp    = current_thread->registers.stack.rbp;
    regs->rsp    = current_thread->registers.stack.rsp;
    regs->ss     = current_thread->registers.stack.ss;

    regs->cs     = current_thread->registers.program.cs;
    regs->rip    = current_thread->registers.program.rip;
    regs->rflags = current_thread->registers.program.rflags;

    regs->pgd    = current_thread->task->mm_info.pgd;

    current_thread->state = RUNNING;
}

//进行调度
PUBLIC void do_switch(struct intterup_args *regs) {
    down_thread(regs);

    current_thread = get_next_run_thread();

    up_thread(regs);

    if (current_thread->task->level == 1) {
        current_thread->count = current_thread->priority * 3;
    }
}

//系统调用之后调用
PUBLIC void after_syscall(struct intterup_args *regs) {
    if (should_switch()) {
        do_switch(regs);
    }
}