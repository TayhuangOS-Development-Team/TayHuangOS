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
 * syscall_func.S
 *
 * INT型系统调用
 *
 */

.extern get_ticks
.type get_ticks, @function
.extern sleep
.type sleep, @function
.extern exit
.type exit, @function
.extern wakeup
.type wakeup, @function
.extern fatal
.type fatal, @function
.extern eggs
.type eggs, @function
.extern signal
.type signal, @function

get_ticks: /*int get_ticks(void)*/
    movq $3, %rax /*sysno = 3(get_ticks)*/

    int $0x40
    ret

sleep: /*int sleep(void)*/
    movq $4, %rax /*sysno = 4(sleep)*/

    int $0x40
    ret

wakeup: /*int wakeup(int pid)*/
    movq %rdi, %r8

    movq $5, %rax /*sysno = 5(wakeup)*/

    int $0x40
    ret

fatal: /*int fatal(void)*/
    movq $6, %rax /*sysno = 6(fatal)*/

    int $0x40
    ret

eggs: /*int eggs(void)*/
    movq $7, %rax /*sysno = 7(eggs)*/

    int $0x40
    ret

signal: /*int signal(int pid, int signal)*/
    movq %rdi, %r8
    movq %rsi, %rdx
    movq $8, %rax /*sysno = 8(signal)*/

    int $0x40
    ret
