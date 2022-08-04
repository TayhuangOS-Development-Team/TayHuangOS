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
#include <task/thread.h>

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

    int pid; //PID
    int level; //调度级别
    int cur_tid;

    word cs;
    word ds;
    qword rflags;
    int priority;
    bool kernel_task;
} task_struct;

#define TASK_INIT_HEAP_SIZE (0x10000) //初始化堆大小