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
 * thread.h
 * 
 * 线程
 * 
 */



#pragma once

#include <tayhuang/defs.h>

//进程的寄存器信息
typedef struct {
    //基本寄存器
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
    //栈寄存器
    struct {
        b64 rbp;
        b64 rsp,
            ss;
    } stack;
    //程序寄存器
    struct {
        b64 rip,
            cs,
            rflags;
    } program;
} register_info_struct;

typedef struct __task_struct task_struct;

typedef struct __thread_info_struct {
    register_info_struct registers;

    //线程状态
    enum {
        READY = 0,
        RUNNING,
        SUBBMITED,
        WAITING,
        WAITING_IPC,
        TERMINATED,
        EXCEPTION,
        SUSPEND,
        WAITING_SUSPEND,
        WAITING_IPC_SUSPEND
    } state;

    struct __thread_info_struct *next;
    struct __thread_info_struct *last;
    //空闲链表
    struct __thread_info_struct *free_next;

    task_struct *task;

    int count; //时间片
    int priority; //优先级

    int tid;
}thread_info_struct;

PUBLIC thread_info_struct *create_thread_info(
    word ds, void *stack_top, void *stack_bottom, void *entry, word cs, qword rflags, int priority, task_struct *task
);

//创建线程
PUBLIC thread_info_struct *__create_thread(void *entry, task_struct *task);

//移除线程
PUBLIC void remove_thread(thread_info_struct *thread);

//根据tid获得线程
PUBLIC thread_info_struct *get_thread_by_tid(int tid, task_struct *task);

//结束线程
PUBLIC void __termintate_thread(thread_info_struct *thread);

#define THREAD_STACK_SIZE (0x800000)      //线程栈大小
#define THREAD_INIT_STACK_SIZE (0x10000) //初始化栈大小