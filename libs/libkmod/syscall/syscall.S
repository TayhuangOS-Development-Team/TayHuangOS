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
    movq $0, %rax /*sysno = 0(moo)*/

    int $0x40
    ret
    