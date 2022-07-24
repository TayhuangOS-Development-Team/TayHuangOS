/*
 * SPDX-License-Identifier: LGPL-2.1-only
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

//进行映射
PUBLIC bool shm_mapping(void *mem, int pages, int src_pid, int target_pid) {
    //获取原页表与新页表
    void *target_pgd = get_task_by_pid(target_pid)->mm_info.pgd;
    void *source_pgd = get_task_by_pid(src_pid)->mm_info.pgd;

    //映射
    bool ret =set_mappingvv(source_pgd, mem, target_pgd, mem, pages, true, true);

    return ret;
}