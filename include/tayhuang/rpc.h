/*
 * SPDX-License-Identifier: GPL-3.0-only
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

typedef int rpc_func;

typedef struct {
    void *data;
    int size;
} rpc_args_struct;

typedef rpc_args_struct(*rpc_proccess_controller)(rpc_func, rpc_args_struct);

#define ARG_READ(args, type) *(type*)(((args) = (((void*)(args)) + sizeof(type))) - sizeof(type))
#define ARG_WRITE(args, type, value) *(type*)(((args) = (((void*)(args)) + sizeof(type))) - sizeof(type)) = value

typedef void *rpc_proccess;

PUBLIC void rpc_register_process_controller(rpc_proccess_controller);
PUBLIC void rpc_register(rpc_func func, rpc_proccess process, int return_size, int args_size); //当args_size = -1时, 不对参数大小进行检验
PUBLIC rpc_args_struct rpc_call(rpc_func func, rpc_args_struct args);