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
PUBLIC task_struct *get_level0_list(void);
PUBLIC task_struct *get_level1_list(void);

//获取空闲进程
PUBLIC task_struct *dequeue_level0_task(void);
PUBLIC task_struct *dequeue_level1_task(void);

//level0是否有task
PUBLIC bool has_level0_task(void);
//level1是否有task
PUBLIC bool has_level1_task(void);

//加入空闲进程
PUBLIC void enqueue_level0_task(task_struct *task);
PUBLIC void enqueue_level1_task(task_struct *task);

PUBLIC task_struct *__create_task(
    word ds, void *stack_top, void *stack_bottom, void *entry, word cs, qword rflags,
    void *pgd, qword code_start, qword code_end, qword data_start, qword data_end, qword heap_start, qword heap_end, qword rodata_start, qword rodata_end,
     qword shm_start, qword shm_end,
    int pid, int priority, int level, task_struct *parent
);

PUBLIC task_struct *create_task(
    word ds, void *stack_top, void *stack_bottom, void *entry, word cs, qword rflags,
    void *pgd, qword code_start, qword code_end, qword data_start, qword data_end, qword heap_start, qword heap_end, qword rodata_start, qword rodata_end,
     qword shm_start, qword shm_end,
    int pid, int priority, int level, task_struct *parent
);