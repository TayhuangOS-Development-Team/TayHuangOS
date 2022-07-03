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
 * task_struct.h
 * 
 * PCB
 * 
 */



#pragma once

#include <tayhuang/defs.h>

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
        gs,
        fs,
        es,
        ds,
        pgd,
        rbp;
    b64 rip,
        cs,
        rflags,
        rsp,
        ss;
} thread_info_struct;

typedef struct {
    void *pgd;
} mm_info_struct;

typedef struct __msgpack_struct {
    union {
        qword value;
        void *address;
    };
    int length;
    struct __msgpack_struct *next;
} msgpack_struct;

#define NULL_TASK (0)
#define ANY_TASK (-1)

typedef struct {
    void *wait_msg;
    int wait_for;
    msgpack_struct *pack_queue_head;
    msgpack_struct *pack_queue_tail;
} ipc_info;

typedef struct __task_struct {
    thread_info_struct thread_info;

    mm_info_struct mm_info;
    ipc_info ipc_info;

    enum {
        READY = 0,
        RUNNING,
        SUBBMITED,
        WAITING,
        WAITING_BECAUSE_SENDING,
        WAITING_BECAUSE_RECEIVING,
        TERMINATED,
        EXCEPTION
    } state;

    int pid;

    struct __task_struct *last;
    struct __task_struct *next;

    int count;
    int priority;
} task_struct;