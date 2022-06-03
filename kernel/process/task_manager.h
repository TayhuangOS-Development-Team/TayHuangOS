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
 * task_manager.h
 *
 * 进程管理器
 *
 */



#pragma once

#include "task_struct.h"

#include <intterup/init_int.h>

PUBLIC extern task_struct *task_table;
PUBLIC extern task_struct *current_task;
PUBLIC extern task_struct *empty_task;
PUBLIC task_struct *create_task(int pid, int priority, void *entry, qword rflags, qword rsp, qword rbp, word cs, void *pgd,
    qword start_pos, qword end_pos, qword start_heap, qword end_heap);
PUBLIC task_struct *do_fork_execv(int priority, void(*entry)(void), qword rsp, qword rbp);
PUBLIC void do_switch(struct intterup_args *regs);
PUBLIC task_struct *find_task(int pid);
PUBLIC void taskman(void);