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
 * task.h
 * 
 * 任务
 * 
 */



#pragma once

#include <tayhuang/defs.h>

typedef struct {
    reg64_t rbx;
    reg64_t rbp;
    reg64_t rdi;
    reg64_t rsi;
    reg64_t r12;
    reg64_t r13;
    reg64_t r14;
    reg64_t r15;
    reg64_t rip;
} stack_struct;

typedef struct {
    stack_struct *rsp;
} task_info_struct;
