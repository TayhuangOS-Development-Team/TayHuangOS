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

PUBLIC bool send_msg(msgno_id msgno, void *msg, qword size, int dst);
PUBLIC void check_ipc(void);
PUBLIC void set_allow(int pid);
PUBLIC msgpack_struct recv_msg(void *msg);
PUBLIC void set_mailbuffer(void *buffer, int size);

typedef void(* normal_ipc_handler_t)(int, void *);
PUBLIC void register_normal_ipc_handler(normal_ipc_handler_t handler);

typedef void(* irq_handler_t)(int);
PUBLIC void register_irq_handler(irq_handler_t handler);

PUBLIC void message_loop(void);

EXTERN PUBLIC int self_pid;
EXTERN PUBLIC word msgid_counter;

static inline word get_msgid(void) {
    if (msgid_counter == 65535) {
        msgid_counter = 0;
        return 65535;
    }
    return msgid_counter ++;
}