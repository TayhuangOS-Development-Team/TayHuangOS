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
    b64 rsp0;
    b16 cs;
    b16 fs;
    b16 gs;
    b16 ss;

    b64 rip;
    b64 rflags;

    b64 rax;
    b64 rbx;
    b64 rcx;
    b64 rdx;
    b64 rsp;
    b64 rbp;
    b64 rsi;
    b64 rdi;
    b64 r8;
    b64 r9;
    b64 r10;
    b64 r11;
    b64 r12;
    b64 r13;
    b64 r14;
    b64 r15;
} thread_struct;

typedef struct _task_struct{
    thread_struct thread_info;
    mm_struct *mm_info;

    volatile enum {
        SUBBMITED, //已提交
        READY, //就绪
        WAITING_SIGNAL, //等待信号
        WAITING_RESOURCE, //等待资源
        RUNNING, //正在运行
        EXCEPTION, //异常
        TERMINATE //结束
    } state;

    int counter;
    int priority;
    int pid;

    struct {
        bool SIGREQ   : 1;  //发起通信请求(建立管道通信)
        bool SIGHUP   : 1;  //结束通信(结束管道通信)
        bool SIGINT   : 1;  //程序中止
        bool SIGQUIT  : 1;  //程序退出
        bool SIGTERM  : 1;  //程序被要求退出
        bool SIGKILL  : 1;  //程序被杀死
        bool SIGSTOP  : 1;  //进程暂停
        bool SIGCONT  : 1;  //进程继续
        bool SIGBLOCK : 1;  //进程被阻塞
        bool SIGALARM : 1;  //警告信号
        bool SIGERROR : 1;  //错误信号
        bool SIGUSER0 : 1;  //用户定义信号
        bool SIGUSER1 : 1;  //用户定义信号
        bool SIGUSER2 : 1;  //用户定义信号
        bool SIGUSER3 : 1;  //用户定义信号
        bool SIGIPCRQ : 1;  //发起IPC通信请求
        bool SIGIPCHP : 1;  //结束IPC通信
        bool SIGANSOK : 1;  //同意应答
        bool SIGANSNO : 1;  //不同意应答
        bool SIGRPT   : 1;  //请求再次发送上个（除SIGANSED）的应答
        bool SIGANSED : 1;  //询问是否已经应答了上个（除SIGANSED）的信号
        bool SIGUSR0S : 1;  //用户定义信号(带发送端)
        bool SIGUSR1S : 1;  //用户定义信号(带发送端)
        bool SIGUSR2S : 1;  //用户定义信号(带发送端)
        bool SIGUSR3S : 1;  //用户定义信号(带发送端)
        bool SIGBREAK : 1;  //进程损坏信号
        qword reserved : 38;
    } signal;

    struct _task_struct *next;
    struct _task_struct *last;
} task_struct;
