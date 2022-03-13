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

    task->counter = 0;
    task->priority = priority;

    task->state = SUBBMITED;
    task->next = task->last = NULL;
    task->pid = pid;
    memset (&task->signal, 0, sizeof (task->signal));
    task->mm_info = create_mm_struct(pgd, start_code, end_code, start_data, end_data,
        start_brk, end_brk, start_rodata, end_rodata, start_stack);
    return task;
}

PRIVATE task_struct *task_table;
PRIVATE int __cur_pid = 0;

PRIVATE int alloc_pid(void) {
    return __cur_pid ++;
}

PUBLIC task_struct *create_task(int priority, void *entry, qword rflags, word cs, void *pgd) {
    task_struct *task = create_task_struct(alloc_pid(), priority, entry, rflags, cs, pgd,
         0, 0, 0, 0, 0 ,0 ,0 ,0 ,0);
    task->next = task_table;
    task_table = task;
    if (task_table != NULL)
        task_table->last = task;
    return task;
}

PUBLIC void do_fork(int priority, void(*entry)(void), qword rflags, word cs, void *pgd) {
    create_task(priority, entry, rflags, cs, pgd);
    //TODO
}

PUBLIC void do_kernel_fork(int priority, void(*entry)(void)) {
    create_task(priority, entry, 1 << 9, rdcs(), get_pml4());
    entry();
}