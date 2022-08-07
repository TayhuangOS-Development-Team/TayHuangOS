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
#include <stdarg.h>

#include <tayhuang/msgpack.h>

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
    proccess_node *new_node = (proccess_node *)(malloc(sizeof(proccess_node)));

    new_node->func_no = func_no;
    new_node->info = info;
    new_node->next = head_node;

    head_node = new_node;
}

PRIVATE void __deal_rpc_request__(msgpack_struct pack, void *msg) {
    rpc_func func_no = ARG_READ(msg, rpc_func);

    proccess_info *info = list_find_proccess(func_no);
    if (info == NULL) {
        lerror ("Invalid rpc request: invalid function no %d", func_no);
        return;
    }

    rpc_size args_size = ARG_READ(msg, rpc_size);
    if (args_size != info->args_size && info->args_size != -1) {
        lerror ("Invalid rpc request: invalid args size %d, should be %d", args_size, info->args_size);
        return;
    }

    rpc_size return_size = ARG_READ(msg, rpc_size);
    if (return_size != info->return_size && info->return_size != -1) {
        lerror ("Invalid rpc request: invalid return size %d, should be %d", return_size, info->return_size);
        return;
    }

    rpc_args_struct args = {.data = (qword)msg, .size = args_size};
    rpc_args_struct result = info->proccess(pack.source, func_no, args);

    void *return_data = malloc(sizeof(rpc_func) + result.size);
    void *_data = return_data;
    ARG_WRITE(_data, rpc_func, func_no);
    memcpy(_data, (void *)result.data, result.size);

    if (info->return_size == result.size || info->return_size == -1) {
        send_msg((msgno_id){.message_no = MSG_RPC_RESULT, .msg_id = pack.msg_id}, return_data, sizeof(rpc_func) + result.size, pack.source);
    }
    else {
        lerror ("RPC: Return size not match!Get %d, Expect %d", result.size, info->return_size);
    }

    free((void *)result.data);
    free(return_data);
}

PRIVATE void __deal_rpc_request(void *args) {
    void *_args = args;
    msgpack_struct pack = ARG_READ(_args, msgpack_struct);
    void *msg = ARG_READ(_args, void *);

    __deal_rpc_request__(pack, msg);

    free(args);
    free(msg);

    exit_thread(NULL);
}

PUBLIC void deal_rpc_request(msgpack_struct pack, void *msg) {
    void *_msg = malloc(pack.length);
    memcpy(_msg, msg, pack.length);

    void *args = malloc(sizeof(msgpack_struct) + sizeof(void *));
    void *_args = args;

    ARG_WRITE(_args, msgpack_struct, pack);
    ARG_WRITE(_args, void *, _msg);

    create_thread(__deal_rpc_request, args);
}

PUBLIC void rpc_register(rpc_func func, rpc_proccess_wrapper process, rpc_size return_size, rpc_size args_size) {
    proccess_info *info = (proccess_info *)(malloc(sizeof(proccess_info)));
    info->func_no = func;
    info->proccess = process;
    info->return_size = return_size;
    info->args_size = args_size;
    add_proccess(func, info);
}

PUBLIC void *prepare_params(rpc_func func, rpc_size return_size, rpc_args_struct args) {
    rpc_size size = sizeof(rpc_func) + sizeof(rpc_size) * 2 + args.size;

    // 准备参数
    void *data = malloc(size);
    void *_data = data;
    ARG_WRITE(_data, rpc_func, func);
    ARG_WRITE(_data, rpc_size, args.size);
    ARG_WRITE(_data, rpc_size, return_size);
    memcpy(_data, args.data, args.size);

    return data;
}

PUBLIC rpc_args_struct __rpc_call(int service, rpc_func func, rpc_args_struct args, rpc_size return_size, void *result) {
    void *params = prepare_params(func, return_size, args);

    //设置信号量
    word id = get_msgid();
    id_t signal = create_signal(2, 0, false);
    void *return_data = malloc(sizeof(rpc_func) + return_size);

    set_rpc_result(id, signal, return_data);
    
    //发送
    send_msg(
        (msgno_id){.message_no = MSG_RPC_CALL, .msg_id = id},
        params,
        sizeof(rpc_func) + sizeof(rpc_size) * 2 + args.size,
        service
    );

    //等待结果
    down_signal(signal);

    //TODO: 添加delete_signal功能并在此处增加
    // delete_signal(signal);
    
    //获得结果
    void *_return_data = return_data;

    //判断是否为目标功能
    rpc_func got_func = ARG_READ(_return_data, rpc_func);
    if (func != got_func) {
        lerror ("RPC: Function not match!Wait %d, Got %d", func, got_func);
        free(return_data);
        free(params);
        return (rpc_args_struct){.data = (qword)NULL, .size = 0};
    }

    //拷贝
    memcpy(result, _return_data, return_size);

    free(return_data);
    free(params);
    return (rpc_args_struct){.data = result, .size = return_size};
}

PUBLIC void *rpc_call(int service, rpc_func func, rpc_args_struct args, rpc_size return_size) {
    static void *result = NULL;
    static int result_sz = 0;

    if (result == NULL) {
        result = malloc(return_size);
        result_sz = return_size;
    }

    if (return_size > result_sz) {
        free(result);
        result = malloc(return_size);
        result_sz = return_size;
    }

    __rpc_call(service, func, args, return_size, result);
    return result;
}