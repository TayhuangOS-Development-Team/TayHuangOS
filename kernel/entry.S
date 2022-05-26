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
 * entry.S
 *
 * 内核入口
 *
 */



    .code64
    .text
    .extern entry

    .global _start
_start: /*入口*/
    finit
    call entry
    xorq %rax, %rax
    jmp .

    .global __set_cr0
    .type __set_cr0, @function
__set_cr0: /*设置cr0*/
    movq %rdi, %cr0
    ret

    .global __get_cr0
    .type __get_cr0, @function
__get_cr0: /*获取cr0*/
    movq %cr0, %rax
    ret

    .global __set_cr2
    .type __set_cr2, @function
__set_cr2: /*设置cr2*/
    movq %rdi, %cr2
    ret

    .global __get_cr2
    .type __get_cr2, @function
__get_cr2: /*获取cr2*/
    movq %cr2, %rax
    ret

    .global __set_cr3
    .type __set_cr3, @function
__set_cr3: /*设置cr3*/
    movq %rdi, %cr3
    ret

    .global __get_cr3
    .type __get_cr3, @function
__get_cr3: /*获取cr3*/
    movq %cr3, %rax
    ret

    .global __set_cr4
    .type __set_cr4, @function
__set_cr4: /*设置cr4*/
    movq %rdi, %cr4
    ret

    .global __get_cr4
    .type __get_cr4, @function
__get_cr4: /*获取cr4*/
    movq %cr4, %rax
    ret

    .global __set_efer
    .type __set_efer, @function
__get_efer: /*设置efer*/
    pushq %rdx
    pushq %rcx
    xorq %rdx, %rdx
    movq $0xC0000080, %rcx /*EFER*/
    rdmsr
    popq %rcx
    popq %rdx
    ret

    .global __get_efer
    .type __get_efer, @function
__set_efer: /*获取efer*/
    pushq %rax
    pushq %rdx
    pushq %rcx
    xorq %rdx, %rdx
    movq %rdi, %rax
    movq $0xC0000080, %rcx /*EFER*/
    wrmsr
    popq %rcx
    popq %rdx
    popq %rax
    ret
