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

//链表头/尾
PRIVATE thread_info_struct *level0_list_head = NULL;
PRIVATE thread_info_struct *level0_list_tail = NULL;
PRIVATE thread_info_struct *level1_list_head = NULL;
PRIVATE thread_info_struct *level1_list_tail = NULL;

//获取空闲进程表
PUBLIC thread_info_struct *get_level0_list(void) {
    return level0_list_head;
}

PUBLIC thread_info_struct *get_level1_list(void) {
    return level1_list_head;
}

//获取空闲进程
PUBLIC thread_info_struct *dequeue_level0_thread(void) {
    thread_info_struct *thread = level0_list_head;

    level0_list_head = thread->free_next;
    if (level0_list_head == NULL) {
        level0_list_tail = NULL;
    }
    thread->free_next = NULL;

    return thread;
}

PUBLIC thread_info_struct *dequeue_level1_thread(void) {
    thread_info_struct *thread = level1_list_head;

    level1_list_head = thread->free_next;
    if (level1_list_head == NULL) {
        level1_list_tail = NULL;
    }
    thread->free_next = NULL;

    return thread;
}

//level0是否有thread
PUBLIC bool has_level0_thread(void) {
    return level0_list_head != NULL;
}

//level1是否有thread
PUBLIC bool has_level1_thread(void) {
    return level1_list_head != NULL;
}

//加入空闲进程
PUBLIC void enqueue_level0_thread(thread_info_struct *thread) {
    if (level0_list_tail == NULL) {
        level0_list_tail = level0_list_head = thread;
        thread->free_next = NULL;
        return;
    }
    level0_list_tail->free_next = thread;
    thread->free_next = NULL;
    level0_list_tail = thread;
}

PUBLIC void enqueue_level1_thread(thread_info_struct *thread) {
    if (level1_list_tail == NULL) {
        level1_list_tail = level1_list_head = thread;
        thread->free_next = NULL;
        return;
    }
    level1_list_tail->free_next = thread;
    thread->free_next = NULL;
    level1_list_tail = thread;
}

PUBLIC void enqueue_thread(thread_info_struct *thread) {
    if (thread->task->level == 0) {
        enqueue_level0_thread(thread);
    }
    else if (thread->task->level == 1) {
        enqueue_level0_thread(thread);
    }
}

//初始化mm_info
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

    mm_info->shm_ptr = shm_start;
}

//初始化ipc_info
PRIVATE void __init_ipc_info(ipc_info_struct *ipc_info) {
    ipc_info->write_ptr = NULL;
    ipc_info->read_ptr = NULL;
    ipc_info->lastest_msg = NULL;
    ipc_info->mail = NULL;
    ipc_info->mail_size = 0;
    ipc_info->used_size = 0;
    ipc_info->allow_pid = NULL_TASK;
    ipc_info->msg_handler_thread = NULL;
}

//创建进程(low level)
PUBLIC task_struct *__create_task(
    word ds, void *stack_top, void *stack_bottom, void *entry, word cs, qword rflags,
    void *pgd, qword code_start, qword code_end, qword data_start, qword data_end, qword heap_start, qword heap_end, qword rodata_start, qword rodata_end,
     qword shm_start, qword shm_end,
    int pid, int priority, int level, task_struct *parent
) {
    task_struct *task = (task_struct *)kmalloc(sizeof(task_struct));

    task->threads = create_thread_info(ds, stack_top, stack_bottom, entry, cs, rflags, priority, task);
    __init_mm_info(&task->mm_info, pgd, code_start, code_end, data_start, data_end, stack_bottom, stack_top,
                        heap_start, heap_end, rodata_start, rodata_end, shm_start, shm_end);
    __init_ipc_info(&task->ipc_info);

    task->last = task->next = NULL;
    
    task->pid = pid;
    task->level = level;
    
    task->parent = parent;
    task->children = NULL;

    task->ds = ds;
    task->cs = cs;
    task->rflags = rflags;
    task->priority = priority;

    return task;
}

//创建进程
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

    //加入队列
    add_task(task);

    task->bro = parent->children;
    parent->children = task;

    //加入空闲队列
    enqueue_thread(task->threads);

    return task;
}
