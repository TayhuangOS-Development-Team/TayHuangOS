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

PUBLIC void *shm_create(int pages, int src_pid) {
    //WIP
    return NULL;
}

PUBLIC bool shm_mapping(void *mem, int pages, int src_pid, int target_pid) {
    void *target_pgd = find_task(target_pid)->mm_info->pgd;
    void *source_pgd = find_task(src_pid)->mm_info->pgd;

    set_pml4(target_pgd);

    for (int i = 0 ; i < pages ; i ++)
        set_mapping(mem + i * MEMUNIT_SZ, __pa(source_pgd, mem + i * MEMUNIT_SZ), 1, true, false);

    return true;
}

PUBLIC void shm_delete(void *addr, int pages, int src_pid) {
    //WIP
}