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
#include <tayhuang/msgpack.h>

typedef struct {
    struct {
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
            ds;
    } basic;
    struct {
        b64 rbp;
        b64 rsp,
            ss;
    } stack;
    struct {
        b64 rip,
            cs,
            rflags;
    } program;
} thread_info_struct;

typedef struct {
    void *pgd;
    qword code_start, code_end;
    qword data_start, data_end;
    qword stack_start, stack_end;
    qword heap_start, heap_end;
    qword rodata_start, rodata_end;
} mm_info_struct;



typedef struct {
    void *mail;
    msgpack_struct *lastest_msg;
    void *write_ptr;
    void *read_ptr;
    qword mail_size;
    qword used_size;
} ipc_info_struct;

#define NULL_TASK (0)
#define ANY_TASK (-1)

typedef struct __task_struct {
    thread_info_struct thread_info;
    mm_info_struct mm_info;
    ipc_info_struct ipc_info;

    enum {
        READY = 0,
        RUNNING,
        SUBBMITED,
        WAITING,
        WAITING_IPC,
        TERMINATED,
        EXCEPTION
    } state;

    struct __task_struct *last;
    struct __task_struct *next;
    struct __task_struct *free_next;
    struct __task_struct *parent;
    struct __task_struct *children;
    struct __task_struct *bro;

    dword pid : 20;
    word count : 7;
    byte priority : 3;
    byte level : 2;
} task_struct;