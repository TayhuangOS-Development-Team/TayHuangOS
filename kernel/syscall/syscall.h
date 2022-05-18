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
 * syscall.h
 *
 * 系统调用
 *
 */



#pragma once

#include <tayhuang/defs.h>

#include "../intterup/init_int.h"

//                       rax        rbx           rcx          rdx         rsi       rdi
PUBLIC qword syscall(int sysno, qword mode, qword counter, qword data, void *src, void *dst,
//      r8         r9           r10         r11         r12         r13         r14         r15
    qword arg1, qword arg2, qword arg3, qword arg4, qword arg5, qword arg6, qword arg7, qword arg8);

PUBLIC qword dosyscall(int sysno, qword mode, qword counter, qword data, void *src, void *dst,
    qword arg1, qword arg2, qword arg3, qword arg4, qword arg5, qword arg6, qword arg7, qword arg8);

PUBLIC bool send_msg(void *msg, int dest, int len, int tickout);
PUBLIC bool receive_msg(void *msg, int source);
PUBLIC int receive_any_msg(void *msg);
PUBLIC int get_ticks(void);
PUBLIC bool sleep(void);
PUBLIC bool exit(void);
PUBLIC bool wakeup(int pid);
PUBLIC bool fatal(void);
PUBLIC int eggs(void);
PUBLIC bool signal(int pid, int signal);

PUBLIC bool sendrecv(void *msg, void *ret, int dest, int len, int tickout);