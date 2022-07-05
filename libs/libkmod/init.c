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
 * init.c
 * 
 * 初始化
 * 
 */



#include <tayhuang/defs.h>
#include <tayhuang/kmod_prototypes.h>
#include <debug/logging.h>

PUBLIC void init(void) {
    register int magic __asm__("rax");
    register void *heap_bottom __asm__("rbx");
    register void *heap_top __asm__("rcx");
    register int pid __asm__("rdx");

    if (magic != KMOD_MAGIC) {
        while (true);
    }

    asmv ("jmp kmod_main");
}