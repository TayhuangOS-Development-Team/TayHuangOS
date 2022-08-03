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
 * thread.c
 * 
 * 线程
 * 
 */



#include <task/thread.h>
#include <task/task_struct.h>
#include <task/task_manager.h>

#include <memory/kheap.h>

#include <string.h>

//创建thread_info
PUBLIC thread_info_struct *create_thread_info(
    word ds, void *stack_top, void *stack_bottom, void *entry, word cs, qword rflags, int priority, task_struct *task
) {
    thread_info_struct *thread_info = (thread_info_struct *)kmalloc(sizeof(thread_info_struct));

    memset (thread_info, 0, sizeof(thread_info_struct));

    //设置主线程信息
    thread_info->registers.basic.gs = thread_info->registers.basic.fs = thread_info->registers.basic.es = thread_info->registers.basic.ds = ds;

    thread_info->registers.stack.rbp = (qword)stack_bottom;
    thread_info->registers.stack.rsp = (qword)stack_top;
    thread_info->registers.stack.ss = ds;

    thread_info->registers.program.rip = (qword)entry;
    thread_info->registers.program.cs = cs;
    thread_info->registers.program.rflags = rflags;

    thread_info->state = SUBBMITED; //提交态

    thread_info->next = thread_info->free_next = NULL;
    thread_info->task = task;

    thread_info->priority = priority;
    thread_info->count = priority * 3;

    return thread_info;
}


PUBLIC thread_info_struct *create_thread(void *stack_top, void *stack_bottom, void *entry, task_struct *task) {
    thread_info_struct *thread = create_thread_info(task->ds, stack_top, stack_bottom, entry, task->cs, task->rflags, task->priority, task);

    thread_info_struct *parent = task->threads;

    while (parent->next != NULL) {
        parent = parent->next;
    }

    parent->next = parent;
    
    enqueue_thread(thread);

    return thread;
}