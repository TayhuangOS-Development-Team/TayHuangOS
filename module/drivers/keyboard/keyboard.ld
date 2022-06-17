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
 * keyboard.ld
 *
 * 键盘驱动LD脚本
 *
 */



ENTRY(_start)
OUTPUT_FORMAT("elf64-x86-64")
OUTPUT_ARCH("i386:x86-64")

SECTIONS
{
    . = 0x3500000;
    .text : { *(.text) }
    .data : { *(.data) }
    .bss : { *(.bss) }
}