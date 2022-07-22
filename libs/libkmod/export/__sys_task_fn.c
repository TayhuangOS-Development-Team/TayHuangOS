/*
 * SPDX-License-Identifier: LGPL-2.1-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 * 
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 * 
 * --------------------------------------------------------------------------------
 * 
 * 作者: theflysong
 * 
 * __sys_task_fn.c
 * 
 * sys_task 函数功能
 * 
 */



#include <export/__sys_task_fn.h>

#include <tayhuang/services.h>

#include <syscall/rpc.h>


PUBLIC void *share_memory(void *addr, int pages, int target) {
    char buffer[SHARE_MEMORY_ARGS_SIZE];
    void *buf = buffer;

    ARG_WRITE(buf, void*, addr);
    ARG_WRITE(buf, int, pages);
    ARG_WRITE(buf, int, target);

    void *new_addr = remote_call(SHARE_MEMORY_RETURN_TYPE, SYSTASK_SERVICE, SHARE_MEMORY_FN, MAKE_ARGS(buffer, SHARE_MEMORY_ARGS_SIZE));

    return new_addr;
}

PUBLIC void *create_share_memory(int pages) {
    void *addr = remote_call(CREATE_SHARE_MEMORY_RETURN_TYPE, SYSTASK_SERVICE, CREATE_SHARE_MEMORY_FN, MAKE_ARGS(&pages, CREATE_SHARE_MEMORY_ARGS_SIZE));
    return addr;
}