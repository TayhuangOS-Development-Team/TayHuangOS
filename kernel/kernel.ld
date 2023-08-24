/*
 * SPDX-License-Identifier: LGPL-2.1-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: theflysong
 *
 * kernel.ld
 *
 * 内核LD脚本
 *
 */



ENTRY(setup)
OUTPUT_FORMAT("elf64-x86-64")
OUTPUT_ARCH("i386:x86-64")

SECTIONS
{
    . = 0x400000;
    .text : { *(.text) }
    .data : { *(.data) }
    .bss : { *(.bss) }
}