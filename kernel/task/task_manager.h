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
 * task_manager.h
 * 
 * 进程管理器
 * 
 */



#pragma once

#include <task/task_struct.h>
#include <global.h>

#define EMPTY_TASK_PID (-1)

//获取空进程
PUBLIC task_struct *get_empty_task(void);
//设置空进程
PUBLIC void create_empty_task(void);

//是否为空进程
static inline bool is_empty(task_struct *task) {
    return task->pid == EMPTY_TASK_PID;
}

//分配pid
static inline int alloc_pid(void) {
    return cur_pid ++;
}

//获取进程表
PUBLIC task_struct *get_task_list(void); 
//获取进程
PUBLIC task_struct *get_task_by_pid(int pid);
//增加进程
PUBLIC void add_task(task_struct *task);
//移除进程
PUBLIC void remove_task(task_struct *task);

//获取空闲进程表
PUBLIC thread_info_struct *get_level0_list(void);
PUBLIC thread_info_struct *get_level1_list(void);

//获取空闲进程
PUBLIC thread_info_struct *dequeue_level0_thread(void);
PUBLIC thread_info_struct *dequeue_level1_thread(void);

//level0是否有thread
PUBLIC bool has_level0_thread(void);
//level1是否有thread
PUBLIC bool has_level1_thread(void);

//加入空闲进程
PUBLIC void enqueue_level0_thread(thread_info_struct *thread);
PUBLIC void enqueue_level1_thread(thread_info_struct *thread);
PUBLIC void enqueue_thread(thread_info_struct *thread);

//创建进程(low level)
PUBLIC task_struct *__create_task(
    word ds, void *stack_top, void *stack_bottom, void *entry, word cs, qword rflags,
    void *pgd, qword code_start, qword code_end, qword data_start, qword data_end, qword heap_start, qword heap_end, qword rodata_start, qword rodata_end,
     qword shm_start, qword shm_end,
    int pid, int priority, int level, task_struct *parent, bool kernel_task
);

//创建进程
PUBLIC task_struct *create_task(
    word ds, void *stack_top, void *stack_bottom, void *entry, word cs, qword rflags,
    void *pgd, qword code_start, qword code_end, qword data_start, qword data_end, qword heap_start, qword heap_end, qword rodata_start, qword rodata_end,
     qword shm_start, qword shm_end,
    int pid, int priority, int level, task_struct *parent, bool kernel_task
);