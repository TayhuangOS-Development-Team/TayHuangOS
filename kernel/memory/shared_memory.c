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
 * shared_memory.c
 * 
 * 共享内存
 * 
 */



#include "shared_memory.h"
#include "pmm.h"
#include "paging.h"
#include <process/task_manager.h>

PUBLIC void *shm_create(int pages) {
    //WIP
}

PUBLIC void *shm_share(void *mem, int pages, int pid) {
    void *target_pgd = find_task(pid)->mm_info->pgd;
    void *source_pgd = current_task->mm_info->pgd;

    set_pml4(target_pgd);

    for (int i = 0 ; i < pages ; i ++)
        set_mapping(mem + i * MEMUNIT_SZ, __pa(source_pgd, mem + i * MEMUNIT_SZ), 1, true, false);

    return mem;
}

PUBLIC void shm_delete(void *addr, int pages) {
    //WIP
}