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
 * ipc.c
 * 
 * IPC
 * 
 */



#include <syscall/ipc.h>

#include <memory/malloc.h>

#include <debug/logging.h>

PRIVATE normal_ipc_handler_t __normal_ipc_handler__ = NULL;

PUBLIC void register_normal_ipc_handler(normal_ipc_handler_t handler) {
    __normal_ipc_handler__ = handler;
}

#define MESSAGE_LEN (1024)

PUBLIC void message_loop(void) {
    static void *msg = NULL;
    if (msg == NULL) {
        msg = malloc(MESSAGE_LEN);
    }

    while (true) {
        check_ipc();
        recvmsg_result_struct result = recv_msg(msg);

        switch(result.message_no) {
        case MSG_NORMAL_IPC: {
            if (__normal_ipc_handler__ != NULL) {
                __normal_ipc_handler__(result.source, msg);
            }
            break;
        }
        case MSG_IRQ_WAKE: {
            break;
        }
        case MSG_RPC_CALL: {
            break;
        }
        case MSG_RPC_RESULT: {
            break;
        }
        }
        
    }
}