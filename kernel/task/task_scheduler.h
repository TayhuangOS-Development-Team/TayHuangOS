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
 * task_scheduler.h
 * 
 * 进程调度器
 * 
 */



#pragma once

#include <task/task_struct.h>
#include <intterup/init_int.h>

EXTERN PUBLIC task_struct *current_task;
PUBLIC void start_schedule(void);
PUBLIC task_struct *get_next_run_task(void);
PUBLIC bool should_switch(void);
PUBLIC void do_switch(struct intterup_args *regs);
PUBLIC void after_syscall(struct intterup_args *regs);