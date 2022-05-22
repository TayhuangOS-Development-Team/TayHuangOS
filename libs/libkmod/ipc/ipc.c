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

int sendrecv(void *msg, void *ret, int dest, int len, int tickout) {
    if (! send_msg(msg, dest, len, tickout))
        return 0;
    if (! recv_msg(ret, dest))
        return 0;
    return 1;
}