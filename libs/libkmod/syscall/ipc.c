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
#include <syscall/rpc.h>

#include <memory/malloc.h>

#include <debug/logging.h>

PRIVATE normal_ipc_handler_t __normal_ipc_handler__ = NULL;
PRIVATE irq_handler_t __irq_handler__ = NULL;

PUBLIC void register_normal_ipc_handler(normal_ipc_handler_t handler) {
    __normal_ipc_handler__ = handler;
}

PUBLIC void register_irq_handler(irq_handler_t handler) {
    __irq_handler__ = handler;
}

typedef struct __handler_node {
    signal_t signal;
    signal_handler_t handler;
    struct __handler_node *next;
} handler_node;

PRIVATE handler_node *list_head;

PRIVATE void add_node(handler_node *node) {
    node->next = list_head;
    list_head = node;
}

PRIVATE signal_handler_t get_handler(signal_t signal) {
    if (list_head == NULL) {
        return NULL;
    }

    handler_node *node = list_head;

    while (node->next != NULL && (node->signal != signal)) {
        node = node->next;
    }

    if (node->signal != signal) {
        return NULL;
    }

    return node->handler;
}

PUBLIC void register_signal_handler_t(signal_t signal, signal_handler_t handler) {
    handler_node *node = (handler_node*)(malloc(sizeof(handler_node)));

    node->signal = signal;
    node->handler = handler;

    add_node(node);
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
        case MSG_RPC_CALL: {
            deal_rpc_request(result.source, msg);
            break;
        }
        case MSG_RPC_RESULT: {
            rpc_tail(result.source, msg);
            break;
        }
        case MSG_IRQ: {
            if (__irq_handler__ != NULL) {
                __irq_handler__(*(int *)msg);
            }
            break;
        }
        case MSG_SIGNAL: {
            signal_handler_t handler = get_handler(*(signal_t*)msg);
            if (handler != NULL) {
                handler();
            }
        }
        }
        
    }
}