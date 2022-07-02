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

#include <tayhuang/services.h>

#include <ipc/ipc.h>
#include <memory/malloc.h>

#define SHM_MAPPING (5)
#define SET_MAPPING_FROM (11)

bool shm_mapping(void *mem, int pages, int pid) {
    qword command[] = {SHM_MAPPING, mem, pages, pid};
    bool status = false;
    sendrecv(command, &status, TASKMAN_SERVICE, sizeof(command), 20);
    return status;
}

bool set_mapping(void *src_addr, void *dst_addr, int pages, int src_pid) {
    qword command[] = {SET_MAPPING_FROM, src_addr, dst_addr, pages, src_pid};
    bool status = false;
    sendrecv(command, &status, TASKMAN_SERVICE, sizeof(command), 20);
    return status;
}

void *shm_share(void *src_addr, int pages, int src_pid) {
    void *target_addr = malloc(pages * MEMUNIT_SZ);
    set_mapping(src_addr, target_addr, pages, src_pid);
    return target_addr;
}

void shm_unshare(void *target_addr) {
    free (target_addr);
}