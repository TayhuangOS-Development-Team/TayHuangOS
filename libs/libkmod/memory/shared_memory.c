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

#include <tayhuang/services.h>
#include <ipc/ipc.h>

void *shm_create(int pages) {
    //WIP
    return NULL;
}

#define SHM_MAPPING (5)

bool shm_mapping(void *mem, int pages, int pid) {
    qword command[] = {SHM_MAPPING, mem, pages, pid};
    bool status = false;
    sendrecv(command, &status, TASKMAN_SERVICE, sizeof(command), 20);
    return status;
}

void shm_delete(void *addr, int pages) {
    //WIP
}