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
 * syscall.S
 * 
 * 系统调用
 * 
 */



    .global moo
    .type moo, @function
moo: /*void moo(void)*/
    movq $0x00, %rax /*sysno = 0(moo)*/

    int $0x40
    ret

    .global setmail_buffer
    .type setmail_buffer, @function
setmail_buffer: /*void setmail_buffer(void *buffer, int size)*/
    movq $0x04, %rax
    movq %rsi, %rcx

    int $0x40
    ret
    