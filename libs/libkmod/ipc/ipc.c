/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 * 
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 * 
 * --------------------------------------------------------------------------------
 * 
 * 作者: Flysong
 * 
 * ipc.c
 * 
 * IPC
 * 
 */



#include "ipc.h"

#include <tayhuang/services.h>
#include <tayhuang/types.h>

int sendrecv(void *msg, void *ret, int dest, int len, int tickout) {
    if (! send_msg(msg, dest, len, tickout))
        return 0;
    if (! recv_msg(ret, dest))
        return 0;
    return 1;
}

#define GET_CURRENT_PID (0)

int get_current_pid(void) {
    qword command[] = {GET_CURRENT_PID};
    int pid = 0;
    sendrecv (command, &pid, GET_CURRENT_PID, sizeof(command), 20);
    return pid;
}