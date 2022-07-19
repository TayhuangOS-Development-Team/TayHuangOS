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
 * int_func.S
 *
 * IPC相关函数
 *
 */



/*FIXME: 重构*/

.global send_msg
.type send_msg, @function
.global recv_msg
.type recv_msg, @function
.global recv_any_msg
.type recv_any_msg, @function
.global recv_any_msg_and_wait
.type recv_any_msg_and_wait, @function

send_msg: /*int send_msg(void *msg, int dest, int len, int tickout)*/
    movq %rsi, %r8
    movq %rdi, %rsi

    movq $0, %rax /*sysno = 0(send_msg)*/

    int $0x40
    ret

recv_msg: /*int recv_msg(void *msg, int source)*/
    movq %rsi, %r8

    movq $1, %rax /*sysno = 1(recv_msg)*/

    int $0x40
    ret

recv_any_msg: /*int recv_any_msg(void *msg)*/
    movq $2, %rax /*sysno = 2(recv_any_msg)*/

    int $0x40
    ret

recv_any_msg_and_wait: /*int recv_any_msg_and_wait(void *msg)*/
    movq $11, %rax /*sysno = 11(recv_any_msg_and_wait)*/

    int $0x40
    ret
