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
 * int_func.S
 *
 * IPC相关函数
 *
 */



.extern send_msg
.type send_msg, @function
.extern recv_msg
.type recv_msg, @function
.extern recv_any_msg
.type recv_any_msg, @function

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
