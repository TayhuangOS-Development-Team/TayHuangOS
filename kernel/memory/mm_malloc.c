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
 * mm_malloc.c
 * 
 * malloc系函数
 * 
 */



#include "mm_malloc.h"
#include "paging.h"
#include "pmm.h"

#include <tayhuang/paging.h>

#include <process/task_manager.h>

#include <debug/logging.h>

PUBLIC void *cpmalloc(int size) {
    int pages = size / MEMUNIT_SZ + ((size % MEMUNIT_SZ) != 0);

    void *addr = find_continuous_freepages(pages);

    for (int i = 0 ; i < pages ; i ++)
        mark_used(addr + i * MEMUNIT_SZ);

    return addr;
}

PUBLIC void cpfree(void *addr, int size) {
    int pages = size / MEMUNIT_SZ + ((size % MEMUNIT_SZ) != 0);

    for (int i = 0 ; i < pages ; i ++)
        mark_unused(addr + i * MEMUNIT_SZ);
}