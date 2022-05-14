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
 * task_manager.c
 *
 * 进程管理器
 *
 */



#include "task_manager.h"
#include "../kheap.h"
#include "../memory/paging.h"
#include <tayhuang/io.h>

PRIVATE mm_struct *create_mm_struct(void *pgd, qword start_code, qword end_code, qword start_data, qword end_data, qword start_brk, qword end_brk,
    qword start_rodata, qword end_rodata, qword start_stack) {
    mm_struct *mm = malloc(sizeof(mm_struct));

    mm->areas = create_vmarea();
    mm->pgd = pgd;

    mm->start_code   = start_code;
    mm->end_code     = end_code;
    mm->start_data   = start_data;
    mm->end_data     = end_data;
    mm->start_brk    = start_brk;
    mm->end_brk      = end_brk;
    mm->start_rodata = start_rodata;
    mm->end_rodata   = end_rodata;
    mm->start_stack  = start_stack;

    return mm;
}

PRIVATE task_struct *create_task_struct(int pid, int priority, void *entry, qword rflags, word cs, void *pgd, qword start_code, qword end_code,
    qword start_data, qword end_data, qword start_brk, qword end_brk, qword start_rodata, qword end_rodata, qword start_stack) {
    task_struct *task = malloc(sizeof(task_struct));
    memset(&task->thread_info, 0, sizeof(task->thread_info));
    task->thread_info.rip = entry;
    task->thread_info.rflags = rflags;
    task->thread_info.cs = cs;
    task->thread_info.ss = cs - 8;
    task->thread_info.ds = cs - 8;

    task->counter = 0;
    task->priority = priority;

    task->state = SUBBMITED;
    task->next = task->last = NULL;
    task->pid = pid;
    memset (&task->signal, 0, sizeof (task->signal));
    task->mm_info = create_mm_struct(pgd, start_code, end_code, start_data, end_data,
        start_brk, end_brk, start_rodata, end_rodata, start_stack);

    task->ipc_info.msg = NULL;
    task->ipc_info.queue = NULL;
    task->ipc_info.wait_for = 0;
    return task;
}

PUBLIC task_struct *task_table;
PRIVATE int __cur_pid = 0;

PRIVATE int alloc_pid(void) {
    return __cur_pid ++;
}

PUBLIC task_struct *create_task(int priority, void *entry, qword rflags, qword rsp, word cs, void *pgd) {
    task_struct *task = create_task_struct(alloc_pid(), priority, entry, rflags, cs, pgd,
         0, 0, 0, 0, 0 ,0 ,0 ,0 ,0);
    task->next = task_table;
    if (task_table != NULL)
        task_table->last = task;
    task_table = task;
    task->thread_info.rsp = rsp;
    return task;
}

PUBLIC void do_fork(int priority, void(*entry)(void), qword rflags, qword rsp, word cs, void *pgd) {
    create_task(priority, entry, rflags, rsp, cs, pgd);
    //TODO
}

PUBLIC task_struct *current_task = NULL;

PUBLIC void do_switch(struct intterup_args *regs) {
    //down task
    current_task->thread_info.rsp = regs->rsp;
    current_task->thread_info.rip = regs->rip;
    current_task->thread_info.cs = regs->cs;
    current_task->thread_info.ds = regs->ds;
    current_task->thread_info.es = regs->es;
    current_task->thread_info.fs = regs->fs;
    current_task->thread_info.gs = regs->gs;
    current_task->thread_info.ss = regs->ss;
    current_task->thread_info.rflags = regs->rflags;
    current_task->mm_info->pgd = (void*)(regs->pgd & 0xFFFFFFFFFFFFF000);
    //common registers
    current_task->thread_info.rax = regs->rax;
    current_task->thread_info.rbx = regs->rbx;
    current_task->thread_info.rcx = regs->rcx;
    current_task->thread_info.rdx = regs->rdx;
    current_task->thread_info.rsi = regs->rsi;
    current_task->thread_info.rdi = regs->rdi;
    current_task->thread_info.rbp = regs->rbp;

    current_task->thread_info.r8 = regs->r8;
    current_task->thread_info.r9 = regs->r9;
    current_task->thread_info.r10 = regs->r10;
    current_task->thread_info.r11 = regs->r11;
    current_task->thread_info.r12 = regs->r12;
    current_task->thread_info.r13 = regs->r13;
    current_task->thread_info.r14 = regs->r14;
    current_task->thread_info.r15 = regs->r15;
    if (current_task->state == RUNNING)
        current_task->state = READY;

    //switch
    do {
        current_task = current_task->next ? current_task->next : task_table;
        //set counter
        current_task->counter = ((current_task->counter >> 1) + current_task->priority);
    } while (current_task->state != READY && current_task->state != SUBBMITED);

    //up task
    regs->rsp = current_task->thread_info.rsp;
    regs->rip = current_task->thread_info.rip;
    regs->cs = current_task->thread_info.cs;
    regs->ss = current_task->thread_info.ss;
    regs->ds = current_task->thread_info.ds;
    regs->es = current_task->thread_info.es;
    regs->fs = current_task->thread_info.fs;
    regs->gs = current_task->thread_info.gs;
    regs->rflags = current_task->thread_info.rflags;
    regs->pgd &= 0xFFF;
    regs->pgd |= (((qword)current_task->mm_info->pgd) & 0xFFFFFFFFFFFFF000);
    //common registers
    regs->rax = current_task->thread_info.rax;
    regs->rbx = current_task->thread_info.rbx;
    regs->rcx = current_task->thread_info.rcx;
    regs->rdx = current_task->thread_info.rdx;
    regs->rsi = current_task->thread_info.rsi;
    regs->rdi = current_task->thread_info.rdi;
    regs->rbp = current_task->thread_info.rbp;

    regs->r8 = current_task->thread_info.r8;
    regs->r9 = current_task->thread_info.r9;
    regs->r10 = current_task->thread_info.r10;
    regs->r11 = current_task->thread_info.r11;
    regs->r12 = current_task->thread_info.r12;
    regs->r13 = current_task->thread_info.r13;
    regs->r14 = current_task->thread_info.r14;
    regs->r15 = current_task->thread_info.r15;
    current_task->state = RUNNING;
}