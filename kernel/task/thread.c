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
#include <memory/pmm.h>
#include <tayhuang/paging.h>

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

    thread_info->tid = task->cur_tid ++;

    return thread_info;
}

PUBLIC thread_info_struct *__create_thread(void *entry, task_struct *task) {
    void *stack_top = (void*)task->mm_info.stack_end;
    void *stack_bottom = stack_top - THREAD_STACK_SIZE; //获得栈
    task->mm_info.stack_end = (qword)stack_bottom;

    //创建线程信息
    thread_info_struct *thread = create_thread_info(task->ds, stack_top, stack_bottom, entry, task->cs, task->rflags, task->priority, task); 

    if (! task->kernel_task) { //不是内核进程
        qword pages = THREAD_INIT_STACK_SIZE / MEMUNIT_SZ;
        void *real_bottom = stack_top - THREAD_INIT_STACK_SIZE;

        alloc_vpages(task->mm_info.pgd, real_bottom, pages); //分配栈
    }

    thread_info_struct *parent = task->threads;

    while (parent->next != NULL) {
        parent = parent->next;
    }

    //入队
    parent->next = parent;
    thread->last = parent;
    
    enqueue_thread(thread);

    return thread;
}

PUBLIC void remove_thread(thread_info_struct *thread) {
    //从列表中删除
    if (thread->last != NULL) {
        thread->last->next = thread->next;
    }
    if (thread->next != NULL) {
        thread->next->last = thread->last;
    }
    kfree(thread);
}

PUBLIC thread_info_struct *get_thread_by_tid(int tid, task_struct *task) {
    for (thread_info_struct *thread = task->threads ; thread != NULL ; thread = thread->next) {
        if (thread->tid == tid) {
            return thread;
        }
    }
    return NULL;
}

PUBLIC void __termintate_thread(thread_info_struct *thread) {
    thread->state = TERMINATED;
}