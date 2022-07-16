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
 * dosyscall.S
 * 
 * 系统调用
 * 
 */



    .global dosyscall
dosyscall: //dosyscall(int sysno, qword mode, qword counter, qword data, void *src, void *dst, qword arg1, qword arg2, qword arg3, qword arg4, qword arg5, qword arg6, qword arg7, qword arg8)
    pushq %rbx
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15

    movq %rdi, %rax
    movq %rsi, %rbx
    xchg %rdx, %rcx
    movq %r8, %rsi
    movq %r9, %rdi
    movq 48(%rsp), %r8
    movq 56(%rsp), %r9
    movq 64(%rsp), %r10
    movq 72(%rsp), %r11
    movq 80(%rsp), %r12
    movq 88(%rsp), %r13
    movq 96(%rsp), %r14
    movq 104(%rsp), %r15
    int $0x40

    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %rbx

    ret
    