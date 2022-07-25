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
 * ipc.h
 * 
 * IPC
 * 
 */



#pragma once

#include <tayhuang/defs.h>
#include <tayhuang/msgpack.h>

PUBLIC bool send_msg(int msgno, void *msg, qword size, int dst);
PUBLIC void check_ipc(void);
PUBLIC void set_allow(int pid);
PUBLIC recvmsg_result_struct recv_msg(void *msg);
PUBLIC void set_mailbuffer(void *buffer, int size);

typedef void(* normal_ipc_handler_t)(int, void *);
PUBLIC void register_normal_ipc_handler(normal_ipc_handler_t handler);

typedef void(* irq_handler_t)(int);
PUBLIC void register_irq_handler(irq_handler_t handler);

PUBLIC void message_loop(void);