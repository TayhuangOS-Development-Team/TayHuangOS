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
 * sys_task.c
 * 
 * 系统调用进程
 * 
 */



#include <syscall/sys_task.h>
#include <syscall/rpc.h>
#include <syscall/syscalls.h>

#include <memory/kheap.h>
#include <memory/paging.h>

#include <task/task_manager.h>

#include <tayhuang/paging.h>

#define MSG_LENGTH (2048)

PUBLIC void *share_memory(void *addr, int pages, int source_pid, int target_pid) {
    task_struct *source_task = get_task_by_pid(source_pid);
    task_struct *target_task = get_task_by_pid(target_pid);

    void *mapping_ptr = target_task->mm_info.shm_ptr;
    target_task->mm_info.shm_ptr += pages * MEMUNIT_SZ;
}

PUBLIC void sys_task(void) {
    void *msg = kmalloc(MSG_LENGTH);
    while (true) {
        check_ipc();
        recvmsg_result_struct result = recv_msg(msg);
        if (result.message_no == MSG_RPC_CALL) {
            deal_rpc_request(result.source, msg);
        }
    }
}