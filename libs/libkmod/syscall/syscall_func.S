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

.global get_ticks
.type get_ticks, @function
.global sleep
.type sleep, @function
.global exit
.type exit, @function
.global wakeup
.type wakeup, @function
.global fatal
.type fatal, @function
.global eggs
.type eggs, @function
.global signal
.type signal, @function
.global wait_irq
.type wait_irq, @function

get_ticks: /*int get_ticks(void)*/
    movq $3, %rax /*sysno = 3(get_ticks)*/

    int $0x40
    ret

sleep: /*int sleep(void)*/
    movq $4, %rax /*sysno = 4(sleep)*/

    int $0x40
    ret

exit: /*int exit(void)*/
    movq $5, %rax /*sysno = 5(exit)*/

    int $0x40
    ud2 /* shouldn't be executed */
    ret

wakeup: /*int wakeup(int pid)*/
    movq %rdi, %r8

    movq $6, %rax /*sysno = 6(wakeup)*/

    int $0x40
    ret

fatal: /*int fatal(void)*/
    movq $7, %rax /*sysno = 7(fatal)*/

    int $0x40
    ret

eggs: /*int eggs(void)*/
    movq $8, %rax /*sysno = 8(eggs)*/

    int $0x40
    ret

signal: /*int signal(int pid, int signal)*/
    movq %rdi, %r8
    movq %rsi, %rdx
    movq $9, %rax /*sysno = 9(signal)*/

    int $0x40
    ret

wait_irq: /*void wait_irq(int irq)*/
    movq %rdi, %rdx

    movq $10, %rax /*sysno = 10(wait_irq)*/

    int $0x40
    ret
    