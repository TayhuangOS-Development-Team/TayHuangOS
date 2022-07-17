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
 * syscall.S
 * 
 * 系统调用
 * 
 */



    .global moo
    .type moo, @function
moo: /*void moo(void)*/
    movq $0x00, %rax 

    int $0x40
    ret

    .global send_msg
    .type send_msg, @function
send_msg: /*bool send_msg(void *msg, qword size, int dst)*/
    movq $0x01, %rax
    movq %rsi, %rcx
    movq %rdi, %rsi

    int $0x40
    ret

    .global check_ipc
    .type check_ipc, @function
check_ipc: /*void check_ipc(void)*/
    movq $0x02, %rax

    int $0x40
    ret

    .global set_allow
    .type set_allow, @function
set_allow: /*void set_allow(int pid)*/
    movq $0x03, %rax
    movq %rdi, %rdx

    int $0x40
    ret

    .global recv_msg
    .type recv_msg, @function
recv_msg: /*int recv_msg(void *msg)*/
    movq $0x04, %rax

    int $0x40
    ret

    .global set_mailbuffer
    .type set_mailbuffer, @function
set_mailbuffer: /*void set_mailbuffer(void *buffer, int size)*/
    movq $0x07, %rax
    movq %rsi, %rcx

    int $0x40
    ret

    .global reg_irq
    .type reg_irq, @function
reg_irq: /*void reg_irq(int irq)*/
    movq $0x0A, %rax
    movq %rdi, %rdx

    int $0x40
    ret
    