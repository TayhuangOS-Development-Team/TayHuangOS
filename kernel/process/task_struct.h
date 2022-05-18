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
 * task_struct.h
 *
 * 进程信息
 *
 */



#pragma once

#include <tayhuang/defs.h>
#include "vmareas.h"
#include <tayhuang/paging.h>

typedef struct {
    PML4E *pgd;
    qword start_code, start_data, start_stack, start_rodata, start_brk;
    qword end_code, end_data, end_rodata, end_brk;
    vm_area *areas;
} mm_struct;

typedef struct {
    b64 r15,
        r14,
        r13,
        r12,
        r11,
        r10,
        r9,
        r8,
        rdi,
        rsi,
        rdx,
        rcx,
        rbx,
        rax,
        rbp,
        rip,
        cs,
        rflags,
        rsp,
        ss,
        ds,
        es,
        fs,
        gs;
} thread_struct;

typedef struct _msgpack_struct {
    void *msg;
    int len;
    int from;
    struct _msgpack_struct *next_msg;
    struct _msgpack_struct *last_msg;
} msgpack_struct;

typedef struct {
    int wait_for;
    int wait_ticks;
    void *msg;
    msgpack_struct *queue;
} ipc_struct;

typedef struct _task_struct {
    thread_struct thread_info;
    mm_struct *mm_info;
    ipc_struct ipc_info;

    volatile enum {
        SUBBMITED, //已提交
        READY, //就绪
        WAITING_WAKEUP, //等待唤醒信号
        WAITING_FOR_SENDING, //等待对方接收(因为发送而阻塞)
        WAITING_FOR_RECEIVING, //等待对方发送(因为接收而阻塞)
        RUNNING, //正在运行
        EXCEPTION, //异常
        TERMINATE //结束
    } state;

    int counter;
    int priority;
    int pid;

    qword signal;

    struct _task_struct *next;
    struct _task_struct *last;
    struct _task_struct *parent;
    struct _task_struct *bro;
    struct _task_struct *child;
} task_struct;
