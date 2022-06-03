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
 * malloc.c
 * 
 * malloc函数
 * 
 */



#include "malloc.h"
#include <services.h>
#include <types.h>
#include <ipc/ipc.h>

#define MM_INIT_HEAP (0)
#define MM_MALLOC (1)
#define MM_FREE (2)

void init_heap(void) {
    qword command[] = {MM_INIT_HEAP};
    send_msg(command, MM_SERVICE, sizeof(command), 20);
}

void *malloc(int size) {
    void *addr;
    int command[] = {MM_MALLOC, size};
    sendrecv(command, &addr, MM_SERVICE, sizeof(command), 20);
    return addr;
}

void free(void *addr) {
    qword command[] = {MM_FREE, (qword)addr};
    send_msg(command, MM_SERVICE, sizeof(command), 20);
}