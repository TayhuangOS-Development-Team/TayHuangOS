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
#include <syscall/syscall.h>
#include <syscall/rpc.h>

#include <memory/malloc.h>

#include <debug/logging.h>

#include <string.h>

PRIVATE normal_ipc_handler_t __normal_ipc_handler__ = NULL;
PRIVATE irq_handler_t __irq_handler__ = NULL;

PUBLIC void register_normal_ipc_handler(normal_ipc_handler_t handler) {
    __normal_ipc_handler__ = handler;
}

PUBLIC void register_irq_handler(irq_handler_t handler) {
    __irq_handler__ = handler;
}

#define MESSAGE_LEN (1024)

PUBLIC int self_pid = 0;
PUBLIC word msgid_counter = 0;

//-------------------------

typedef struct {
    id_t signal;
    void *data;
} signal_data;

typedef struct __signal_ln {
    int msgid;
    signal_data data;
    struct __signal_ln *next;
} signal_ln;

PRIVATE signal_ln *head;

PUBLIC void set_rpc_result(int msgid, id_t signal, void *data) {
    if (head == NULL) {
        head = (signal_ln*)malloc(sizeof(signal_ln));
        head->msgid = msgid;
        head->data.signal = signal;
        head->data.data = data;
        head->next = NULL;
        return;
    }

    signal_ln *cur = head;

    while (cur->next != NULL) {
        cur = cur->next;
    }

    if (cur->msgid == msgid) {
        head->data.signal = signal;
        head->data.data = data;
        return;
    }

    cur->next = (signal_ln*)malloc(sizeof(signal_ln));
    cur->next->msgid = msgid;
    cur->next->data.signal = signal;
    cur->next->data.data = data;
    cur->next->next = NULL;
}

PUBLIC signal_data *get_signal(int msgid) {
    signal_ln *cur = head;

    while (cur != NULL) {
        if (cur->msgid == msgid) {
            return &cur->data;
        }
        cur = cur->next;
    }

    return NULL;
}

PUBLIC void remove_rpc_result(int msgid) {
    if (head == NULL) {
        return;
    }

    signal_ln *cur = head;
    while (cur->next) {
        if (cur->next->msgid == msgid) {
            signal_ln *nxt = cur->next;
            cur->next = nxt->next;
            free(nxt);
            break;
        }
    }
}

//-------------------------

PUBLIC void message_loop(void) {
    static void *msg = NULL;
    if (msg == NULL) {
        msg = malloc(MESSAGE_LEN);
    }

    while (true) {
        check_ipc();
        msgpack_struct result = recv_msg(msg);

        switch(result.message_no) {
        case MSG_NORMAL_IPC: {
            if (__normal_ipc_handler__ != NULL) {
                __normal_ipc_handler__(result.source, msg);
            }
            break;
        }
        case MSG_RPC_CALL: {
            deal_rpc_request(result, msg);
            break;
        }
        case MSG_RPC_RESULT: {
            signal_data *signal = get_signal(result.msg_id);
            if (signal != NULL) {
                memcpy(signal->data, msg, result.length);
                up_signal(signal->signal);
                remove_rpc_result(result.msg_id);
            }
            break;
        }
        case MSG_IRQ: {
            if (__irq_handler__ != NULL) {
                __irq_handler__(*(int *)msg);
            }
            break;
        }
        }
        
    }
}