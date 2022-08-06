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
 * entry.S
 *
 * 内核模块入口
 *
 */



    .code64
    .text

    .extern __kmod_init__
    .global _start
_start:
    jmp __kmod_init__
