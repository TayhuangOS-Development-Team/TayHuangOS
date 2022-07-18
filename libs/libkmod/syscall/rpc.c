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
#include <syscall/ipc.h>
#include <syscall/syscall.h>

#include <memory/malloc.h>

#include <debug/logging.h>

#include <string.h>

#include <tayhuang/msgpack.h>

#include <task/task.h>

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
    proccess_node *new_node = (proccess_node*)(malloc(sizeof(proccess_node)));

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

    void *return_data = malloc(sizeof(rpc_func) + result.size);
    void *_data = return_data;
    ARG_WRITE(_data, rpc_func, func_no);
    memcpy(_data, (void*)result.data, result.size);

    if (info->return_size == result.size) {
        send_msg(MSG_RPC_RESULT, return_data, sizeof(rpc_func) + result.size, caller);
    }

    free((void*)result.data);
    free(return_data);
}

PUBLIC void rpc_register(rpc_func func, rpc_proccess_wrapper process, rpc_size return_size, rpc_size args_size) {
    proccess_info *info = (proccess_info*)(malloc(sizeof(proccess_info)));
    info->func_no = func;
    info->proccess = process;
    info->return_size = return_size;
    info->args_size = args_size;
    add_proccess(func, info);
}

PRIVATE task_info_struct wait_result_task;

PUBLIC void rpc_mid(int service, rpc_func func, void *retaddr, stack_struct *stack_addr) {
    wait_result_task.rsp = stack_addr;

    message_loop();
}

PUBLIC rpc_args_struct __rpc_call__(int service, rpc_func func, rpc_args_struct args, rpc_size return_size, void *result, void *retaddr, stack_struct *stack_addr) {
    rpc_mid(service, func, retaddr, stack_addr);
    //TODO: 完成 rpc_call
    return (rpc_args_struct){.data = (qword)NULL, .size = 0};
}