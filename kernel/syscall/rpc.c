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
 * rpc.c
 * 
 * 远程过程调用
 * 
 */



#include <syscall/rpc.h>
#include <syscall/syscalls.h>

#include <memory/kheap.h>

#include <logging.h>

#include <string.h>

typedef struct  {
    rpc_func func_no;
    rpc_proccess_wrapper proccess;
    rpc_size return_size;
    rpc_size args_size;
} proccess_info;

//TODO: 使用红黑树
typedef struct __proccess_node {
    rpc_func func_no;
    proccess_info *info;
    struct __proccess_node *next;
} proccess_node;

PRIVATE proccess_node *head_node = NULL;

PRIVATE proccess_info *list_find_proccess(rpc_func func_no) {
    proccess_node *cur_node = head_node;
    while (cur_node != NULL && cur_node->func_no != func_no) {
        cur_node = cur_node->next;
    }
    if (cur_node == NULL)
        return NULL;
    return cur_node->info;
}

PRIVATE void add_proccess(rpc_func func_no, proccess_info *info) {
    proccess_node *new_node = (proccess_node*)(kmalloc(sizeof(proccess_node)));

    new_node->func_no = func_no;
    new_node->info = info;
    new_node->next = head_node;
}

PUBLIC void deal_rpc_request(void *msg, int caller) {
    rpc_func func_no = ARG_READ(msg, rpc_func);

    proccess_info *info = list_find_proccess(func_no);
    if (info == NULL) {
        return;
    }

    rpc_size args_size = ARG_READ(msg, rpc_size);
    if (args_size != info->args_size) {
        return;
    }

    rpc_size return_size = ARG_READ(msg, rpc_size);
    if (return_size != info->return_size) {
        return;
    }

    rpc_args_struct args = {.data = (qword)msg, .size = args_size};
    rpc_args_struct result = info->proccess(caller, func_no, args);

    if (info->return_size == result.size) {
        send_msg(MSG_RPC_RESULT, (void*)result.data, result.size, caller);
    }

    void *return_data = kmalloc(sizeof(rpc_func) + result.size);
    void *_data = return_data;
    ARG_WRITE(_data, rpc_func, func_no);
    memcpy(_data, (void*)result.data, result.size);

    if (info->return_size == result.size) {
        send_msg(MSG_RPC_RESULT, return_data, sizeof(rpc_func) + result.size, caller);
    }

    kfree((void*)result.data);
    kfree(return_data);
}

PUBLIC void rpc_register(rpc_func func, rpc_proccess_wrapper process, rpc_size return_size, rpc_size args_size) {
    proccess_info *info = (proccess_info*)(kmalloc(sizeof(proccess_info)));
    info->func_no = func;
    info->proccess = process;
    info->return_size = return_size;
    info->args_size = args_size;
    add_proccess(func, info);
}

PUBLIC rpc_args_struct rpc_call(int service, rpc_func func, rpc_args_struct args, rpc_size return_size, void *result) {
    // sys_task不应该调用其它进程
    lerror ("RPC", "RPC Call shouldn't be called by sys_task");

    // __rpc_call(service, func, args, return_size);
    // rpc_call_mid(); //作用: 保存当前函数的返回指针 并进入消息循环 直至接收到返回值消息
    return (rpc_args_struct){.data = (qword)NULL, .size = 0};
}

PUBLIC rpc_args_struct rpc_direct_call(int service, rpc_func func, rpc_args_struct args, rpc_size return_size, void *result) {
    lwarn ("RPC", "RPC Call shouldn't be called by sys_task");

    rpc_size size = sizeof(rpc_func) + sizeof(rpc_size) * 2 + args.size;
    void *data = kmalloc(size);
    void *_data = data;
    ARG_WRITE(_data, rpc_func, func);
    ARG_WRITE(_data, rpc_size, args.size);
    ARG_WRITE(_data, rpc_size, return_size);
    memcpy(_data, args.data, args.size);
    send_msg(MSG_RPC_CALL, data, size, service);
    kfree (data);
    check_ipc();
    void *__result = kmalloc(return_size + sizeof(rpc_func));
    recv_msg(__result);
    memcpy(result, __result + sizeof(rpc_func), return_size);
    kfree(__result);

    return (rpc_args_struct){.data = (qword)result, .size = return_size};
}