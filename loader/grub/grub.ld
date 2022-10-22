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
 * grub.ld
 *
 * 加载器LD脚本
 *
 */



ENTRY(entry)
OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH("i386")

SECTIONS
{
    . = 0x2008000;
    .text : {
        . = ALIGN(8);
        KEEP(*(.multiboot));
        *(.text)
    }
    .data : { *(.data) }
    .bss : { *(.bss) }
}