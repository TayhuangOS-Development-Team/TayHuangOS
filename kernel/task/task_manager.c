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
 * task_manager.c
 * 
 * 进程管理器
 * 
 */



#include <task/task_manager.h>
#include <memory/kheap.h>
#include <string.h>
#include <tayhuang/io.h>

PRIVATE task_struct *empty_task = NULL;

//获取空进程
PUBLIC task_struct *get_empty_task(void) {
    return empty_task;
}

PRIVATE void empty(void) {
    while (true);
}

//设置空进程
PUBLIC void create_empty_task(void) {
    empty_task = __create_task(
        DS_KERNEL, NULL, NULL, empty, CS_KERNEL, RFLAGS_KERNEL,
        kernel_pml4,
        -1, -1, -1
    );
}

PRIVATE task_struct *task_list = NULL;

//获取进程表
PUBLIC task_struct *get_task_list(void) {
    return task_list;
}

//获取进程
PUBLIC task_struct *get_task_by_pid(int pid) {
    for (task_struct *task = task_list ; task != NULL ; task = task->next) {
        if (task->pid == pid) {
            return task;
        }
    }
    return NULL;
}

//增加进程
PUBLIC void add_task(task_struct *task) {
    task->next = task_list;
    task->last = NULL;
    if (task_list != NULL) {
        task_list->last = task;
    }
    task_list = task;
}

//移除进程
PUBLIC void remove_task(task_struct *task) {
    if (task->next != NULL) {
        task->next->last = task->last;
    }

    if (task->last != NULL) {
        task->last->next = task->next;
    }
    else {
        task_list = task->next;
    }

    kfree(task);
}


PRIVATE task_struct *level0_list_head = NULL;
PRIVATE task_struct *level0_list_tail = NULL;
PRIVATE task_struct *level1_list_head = NULL;
PRIVATE task_struct *level1_list_tail = NULL;

//获取空闲进程表
PUBLIC task_struct *get_level0_list(void) {
    return level0_list_head;
}

PUBLIC task_struct *get_level1_list(void) {
    return level1_list_head;
}

//获取空闲进程
PUBLIC task_struct *dequeue_level0_task(void) {
    task_struct *task = level0_list_head;
    if (task == NULL)
        return NULL;
    level0_list_head = task->free_next;
    if (task->free_next == NULL) {
        level0_list_tail = NULL;
    }
    return task;
}

PUBLIC task_struct *dequeue_level1_task(void) {
    task_struct *task = level1_list_head;
    if (task == NULL)
        return NULL;
    level1_list_head = task->free_next;
    if (task->free_next == NULL) {
        level1_list_tail = NULL;
    }
    return task;
}

//加入空闲进程
PUBLIC void enqueue_level0_task(task_struct *task) {
    task->free_next = NULL;

    if (level0_list_tail == NULL) {
        level0_list_head = level0_list_tail = task;
        return;
    }

    level0_list_tail->free_next = task;
    level0_list_tail = task;
}

PUBLIC void enqueue_level1_task(task_struct *task) {
    task->free_next = NULL;

    if (level1_list_tail == NULL) {
        level1_list_head = level1_list_tail = task;
        return;
    }

    level1_list_tail->free_next = task;
    level1_list_tail = task;
}

PRIVATE void __init_thread_info(thread_info_struct *thread_info, word ds, void *stack_top, void *stack_bottom, void *entry, word cs, qword rflags) {
    memset (thread_info, 0, sizeof(thread_info_struct));

    thread_info->basic.gs = thread_info->basic.fs = thread_info->basic.es = thread_info->basic.ds = ds;

    thread_info->stack.rbp = (qword)stack_bottom;
    thread_info->stack.rsp = (qword)stack_top;
    thread_info->stack.ss = ds;

    thread_info->program.rip = (qword)entry;
    thread_info->program.cs = cs;
    thread_info->program.rflags = rflags;
}

PRIVATE void __init_mm_info(mm_info_struct *mm_info, void *pgd) {
    mm_info->pgd = pgd;
}

PRIVATE void __init_ipc_info(ipc_info_struct *ipc_info) {
    ipc_info->len = 0;
    ipc_info->source = 0;
    ipc_info->wait_msg = NULL;
    ipc_info->pack_queue_head = ipc_info->pack_queue_tail = NULL;
}

PUBLIC task_struct *__create_task(
    word ds, void *stack_top, void *stack_bottom, void *entry, word cs, qword rflags,
    void *pgd,
    int pid, int priority, int level
) {
    task_struct *task = kmalloc(sizeof(task_struct));

    __init_thread_info(&task->thread_info, ds, stack_top, stack_bottom, entry, cs, rflags);
    __init_mm_info(&task->mm_info, pgd);
    __init_ipc_info(&task->ipc_info);

    task->state = SUBBMITED;
    task->last = task->next = task->free_next = NULL;
    
    task->pid = pid;
    task->count = 0;
    task->priority = priority;
    task->level = level;

    return task;
}

PUBLIC task_struct *create_task(
    word ds, void *stack_top, void *stack_bottom, void *entry, word cs, qword rflags,
    void *pgd,
    int pid, int priority, int level
) {
    task_struct *task = __create_task(ds, stack_top, stack_bottom, entry, cs, rflags,
                                        pgd,
                                        pid, priority, level);

    add_task(task);

    if (level == 0) {
        enqueue_level0_task(task);
    }
    else if (level == 1) {
        enqueue_level1_task(task);
    }

    return task;
}
