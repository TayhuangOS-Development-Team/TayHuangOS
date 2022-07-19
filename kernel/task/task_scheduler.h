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
 * task_scheduler.h
 * 
 * 进程调度器
 * 
 */



#pragma once

#include <task/task_struct.h>
#include <intterup/init_int.h>

//当前进程
EXTERN PUBLIC task_struct *current_task;
//开始调度
PUBLIC void start_schedule(void);
//获得下一个运行的进程
PUBLIC task_struct *get_next_run_task(void);
//是否需要调度
PUBLIC bool should_switch(void);
//进行调度
PUBLIC void do_switch(struct intterup_args *regs);
//系统调用之后
PUBLIC void after_syscall(struct intterup_args *regs);