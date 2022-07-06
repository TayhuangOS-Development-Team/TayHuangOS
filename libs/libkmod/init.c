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
#include <memory/malloc.h>

PUBLIC void __kmod_init__(void) {
    register int magic __asm__("rax");
    register void *heap_start __asm__("rbx");
    register void *heap_end __asm__("rcx");
    register int pid __asm__("rdx");

    set_logging_name("KMod Init");
    init_heap(pid, heap_start, heap_end);

    if (magic != KMOD_MAGIC) {
        while (true);
    }

    kmod_main();

    while (true);
    //exit();
}