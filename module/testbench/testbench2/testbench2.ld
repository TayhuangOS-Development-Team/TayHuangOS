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
 * testbench2.ld
 *
 * testbench2 LD脚本
 *
 */



ENTRY(_start)
OUTPUT_FORMAT("elf64-x86-64")
OUTPUT_ARCH("i386:x86-64")

SECTIONS
{
    . = 0x20000000;
    .text : { *(.text) }
    .data : { *(.data) }
    .bss : { *(.bss) }
}