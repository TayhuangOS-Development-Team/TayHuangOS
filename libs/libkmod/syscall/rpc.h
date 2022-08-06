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
 * rpc.h
 * 
 * 远程过程调用
 * 
 */



#pragma once

#include <tayhuang/defs.h>
#include <tayhuang/msgpack.h>

typedef qword rpc_func;
typedef dword rpc_size;

typedef struct {
    qword data : 44;
    int size : 20;
} rpc_args_struct;

typedef rpc_args_struct(* rpc_proccess_wrapper)(int, rpc_func, rpc_args_struct);

#define ARG_READ(args, type) *(type *)(((args) = (((void *)(args)) + sizeof(type))) - sizeof(type))
#define ARG_WRITE(args, type, value) *(type *)(((args) = (((void *)(args)) + sizeof(type))) - sizeof(type)) = value

#define MAKE_ARGS(args, sz) ((rpc_args_struct){.data = (args), .size = (sz)})

PUBLIC void deal_rpc_request(msgpack_struct pack, void *msg);
PUBLIC void rpc_register(rpc_func func, rpc_proccess_wrapper process, rpc_size return_size, rpc_size args_size); //当args_size = -1时, 不对参数大小进行检验
PUBLIC rpc_args_struct __rpc_call(int service, rpc_func func, rpc_args_struct args, rpc_size return_size, void *result);
PUBLIC void *rpc_call(int service, rpc_func func, rpc_args_struct args, rpc_size return_size);

#define remote_call(ret_type, service, func, args) (*(ret_type *)rpc_call(service, func, args, sizeof(ret_type)))