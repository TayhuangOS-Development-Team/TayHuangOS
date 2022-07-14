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



#include <memory/shared_memory.h>
#include <memory/pmm.h>
#include <memory/paging.h>
#include <task/task_manager.h>

PUBLIC bool shm_mapping(void *mem, int pages, int src_pid, int target_pid) {
    void *target_pgd = get_task_by_pid(target_pid)->mm_info.pgd;
    void *source_pgd = get_task_by_pid(src_pid)->mm_info.pgd;

    for (int i = 0 ; i < pages ; i ++) {
        set_mapping(target_pgd, mem + i * MEMUNIT_SZ, __pa(source_pgd, mem + i * MEMUNIT_SZ), 1, true, false);
    }

    return true;
}