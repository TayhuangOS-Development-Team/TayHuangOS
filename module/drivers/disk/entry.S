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
 * 硬盘驱动入口
 *
 */



    .code64
    .text
    .extern entry

    .global _start
_start:
    pushq %rax
    jmp .
