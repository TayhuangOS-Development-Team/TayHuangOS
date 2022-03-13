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

PUBLIC task_struct *create_task(int priority, void *entry, qword rflags, word cs, void *pgd);
PUBLIC void do_fork(int priority, void(*entry)(void), qword rflags, word cs, void *pgd);
PUBLIC void do_kernel_fork(int priority, void(*entry)(void));