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
 * task_struct.h
 * 
 * PCB
 * 
 */



#pragma once

#include <tayhuang/defs.h>
#include <tayhuang/msgpack.h>

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
        TERMINATED,
        EXCEPTION
    } state;

    struct __thread_info_struct *next;
    //空闲链表
    struct __thread_info_struct *free_next;

    task_struct *task;

    word count : 10; //时间片
    byte priority : 6; //优先级
}thread_info_struct;

//内存管理信息
typedef struct {
    void *pgd;
    qword code_start, code_end; //代码段
    qword data_start, data_end; //数据段
    qword stack_start, stack_end; //栈段
    qword heap_start, heap_end; //堆段
    qword rodata_start, rodata_end; //只读数据段
    qword shm_start, shm_end; //共享内存区
    qword shm_ptr; //共享内存指针
} mm_info_struct;

//特殊PID
#define NULL_TASK (0)
#define ANY_TASK (-1)
#define DUMMY_TASK (-2)

//IPC信息
typedef struct {
    void *mail; //邮箱
    msgpack_struct *lastest_msg; //最新消息
    void *write_ptr; //写入指针
    void *read_ptr; //读入指针
    qword mail_size; //邮箱大小
    qword used_size; //已使用大小
    int allow_pid; //允许的PID
    thread_info_struct *msg_handler_thread; //消息处理线程
} ipc_info_struct;

typedef struct __task_struct {
    //进程信息
    thread_info_struct *threads;
    mm_info_struct mm_info;
    ipc_info_struct ipc_info;

    //进程链表
    struct __task_struct *last;
    struct __task_struct *next;

    //进程树
    struct __task_struct *parent;
    struct __task_struct *children;
    struct __task_struct *bro;


    int pid : 28; //PID
    byte level : 4; //调度级别
} task_struct;