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
#include <memory/pmm.h>

#include <task/task_manager.h>

#include <tayhuang/paging.h>
#include <tayhuang/services.h>

#include <assert.h>
#include <logging.h>

#include <export/__sys_task_fn.h>

#define MSG_LENGTH (2048)

//共享内存
PRIVATE SHARE_MEMORY_RETURN_TYPE __share_memory(void *addr, int pages, int source_pid, int target_pid) {
    task_struct *source_task = get_task_by_pid(source_pid);
    task_struct *target_task = get_task_by_pid(target_pid);

    void *mapping_ptr = target_task->mm_info.shm_ptr; //获取指针
    target_task->mm_info.shm_ptr += pages * MEMUNIT_SZ;

    set_mappingvv(source_task->mm_info.pgd, addr, target_task->mm_info.pgd, mapping_ptr, pages, true, true);

    return mapping_ptr;
}

PRIVATE rpc_args_struct wrapper_share_memory(int caller, rpc_func func_no, rpc_args_struct args) {
    void *addr = ARG_READ(args.data, void*);
    int pages = ARG_READ(args.data, int);
    int target = ARG_READ(args.data, int);
    
    void **dst = (SHARE_MEMORY_RETURN_TYPE*)kmalloc(sizeof(SHARE_MEMORY_RETURN_TYPE));
    *dst = __share_memory(addr, pages, caller, target);

    return (rpc_args_struct){.data = dst, .size = sizeof(SHARE_MEMORY_RETURN_TYPE)};
}

//创建共享内存
PRIVATE CREATE_SHARE_MEMORY_RETURN_TYPE __create_share_memory(int pages, int source_pid) {
    task_struct *source_task = get_task_by_pid(source_pid);

    void *mapping_ptr = source_task->mm_info.shm_ptr; //获取指针
    source_task->mm_info.shm_ptr += pages * MEMUNIT_SZ;

    alloc_vpages(source_task->mm_info.pgd, mapping_ptr, pages); //分配

    return mapping_ptr;
}

PRIVATE rpc_args_struct wrapper_create_share_memory(int caller, rpc_func func_no, rpc_args_struct args) {
    int pages = ARG_READ(args.data, int);
    
    void **dst = (CREATE_SHARE_MEMORY_RETURN_TYPE*)kmalloc(sizeof(CREATE_SHARE_MEMORY_RETURN_TYPE));
    *dst = __create_share_memory(pages, caller);

    return (rpc_args_struct){.data = dst, .size = sizeof(CREATE_SHARE_MEMORY_RETURN_TYPE)};
}

PUBLIC void sys_task(void) {
    void *mail = kmalloc(8192);
    set_mailbuffer(mail, 8192); //初始化邮箱

    //注册rpc函数
    rpc_register(SHARE_MEMORY_FN, wrapper_share_memory, sizeof(SHARE_MEMORY_RETURN_TYPE), SHARE_MEMORY_ARGS_SIZE);
    rpc_register(CREATE_SHARE_MEMORY_FN, wrapper_create_share_memory, sizeof(CREATE_SHARE_MEMORY_RETURN_TYPE), CREATE_SHARE_MEMORY_ARGS_SIZE);

    set_allow(ANY_TASK);

    void *msg = kmalloc(MSG_LENGTH);

    //初始化完成
    bool status = true;
    assert(send_msg(MSG_NORMAL_IPC, status, sizeof(bool), INIT_SERVICE));

    while (true) {
        //接收消息
        check_ipc();
        recvmsg_result_struct result = recv_msg(msg);
        //是RPC调用
        if (result.message_no == MSG_RPC_CALL) {
            deal_rpc_request(result.source, msg);
        }
    }
}