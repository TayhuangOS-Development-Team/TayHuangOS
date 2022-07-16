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

#include <global.h>

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
        DS_KERNEL, RING0_STACKTOP3, RING0_STACKBOTTOM3, empty, CS_KERNEL, RFLAGS_KERNEL,
        kernel_pml4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        -1, -1, -1, NULL
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

    level0_list_head = task->free_next;
    if (level0_list_head == NULL) {
        level0_list_tail = NULL;
    }
    task->free_next = NULL;

    return task;
}

PUBLIC task_struct *dequeue_level1_task(void) {
    task_struct *task = level1_list_head;

    level1_list_head = task->free_next;
    if (level1_list_head == NULL) {
        level1_list_tail = NULL;
    }
    task->free_next = NULL;

    return task;
}

//level0是否有task
PUBLIC bool has_level0_task(void) {
    return level0_list_head != NULL;
}

//level1是否有task
PUBLIC bool has_level1_task(void) {
    return level1_list_head != NULL;
}

//加入空闲进程
PUBLIC void enqueue_level0_task(task_struct *task) {
    if (level0_list_tail == NULL) {
        level0_list_tail = level0_list_head = task;
        task->free_next = NULL;
        return;
    }
    level0_list_tail->free_next = task;
    task->free_next = NULL;
    level0_list_tail = task;
}

PUBLIC void enqueue_level1_task(task_struct *task) {
    if (level1_list_tail == NULL) {
        level1_list_tail = level1_list_head = task;
        task->free_next = NULL;
        return;
    }
    level1_list_tail->free_next = task;
    task->free_next = NULL;
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

PRIVATE void __init_mm_info(mm_info_struct *mm_info, void *pgd, qword code_start, qword code_end, qword data_start, qword data_end, qword stack_start, qword stack_end,
        qword heap_start, qword heap_end, qword rodata_start, qword rodata_end, qword shm_start, qword shm_end) {
    mm_info->pgd = pgd;

    mm_info->code_start = code_start;
    mm_info->code_end = code_end;

    mm_info->data_start = data_start;
    mm_info->data_end = data_end;

    mm_info->stack_start = stack_start;
    mm_info->stack_end = stack_end;

    mm_info->heap_start = heap_start;
    mm_info->heap_end = heap_end;

    mm_info->rodata_start = rodata_start;
    mm_info->rodata_end = rodata_end;

    mm_info->shm_start = shm_start;
    mm_info->shm_end = shm_end;
}

PRIVATE void __init_ipc_info(ipc_info_struct *ipc_info) {
    ipc_info->write_ptr = NULL;
    ipc_info->read_ptr = NULL;
    ipc_info->lastest_msg = NULL;
    ipc_info->mail = NULL;
    ipc_info->mail_size = 0;
    ipc_info->used_size = 0;
    ipc_info->allow_pid = NULL_TASK;
}

PUBLIC task_struct *__create_task(
    word ds, void *stack_top, void *stack_bottom, void *entry, word cs, qword rflags,
    void *pgd, qword code_start, qword code_end, qword data_start, qword data_end, qword heap_start, qword heap_end, qword rodata_start, qword rodata_end,
     qword shm_start, qword shm_end,
    int pid, int priority, int level, task_struct *parent
) {
    task_struct *task = kmalloc(sizeof(task_struct));

    __init_thread_info(&task->thread_info, ds, stack_top, stack_bottom, entry, cs, rflags);
    __init_mm_info(&task->mm_info, pgd, code_start, code_end, data_start, data_end, stack_bottom, stack_top,
                        heap_start, heap_end, rodata_start, rodata_end, shm_start, shm_end);
    __init_ipc_info(&task->ipc_info);

    task->state = SUBBMITED;
    task->last = task->next = task->free_next = NULL;
    
    task->pid = pid;
    task->count = 0;
    task->priority = priority;
    task->level = level;
    
    task->parent = parent;
    task->children = NULL;

    return task;
}

PUBLIC task_struct *create_task(
    word ds, void *stack_top, void *stack_bottom, void *entry, word cs, qword rflags,
    void *pgd, qword code_start, qword code_end, qword data_start, qword data_end, qword heap_start, qword heap_end, qword rodata_start, qword rodata_end,
     qword shm_start, qword shm_end,
    int pid, int priority, int level, task_struct *parent
) {
    task_struct *task = __create_task(ds, stack_top, stack_bottom, entry, cs, rflags,
                                        pgd, code_start, code_end, data_start, data_end, heap_start, heap_end, rodata_start, rodata_end,
                                         shm_start, shm_end,
                                        pid, priority, level, parent);

    add_task(task);

    task->bro = parent->children;
    parent->children = task;

    if (level == 0) {
        enqueue_level0_task(task);
    }
    else if (level == 1) {
        enqueue_level1_task(task);
    }

    return task;
}
