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
 * msgpack.h
 * 
 * 消息包结构
 * 
 */



#pragma once

#include <tayhuang/defs.h>

typedef struct {
    qword length : 30;
    byte message_no : 6; 
    dword source : 28;
} msgpack_struct;

#define MSG_NORMAL_IPC    (0x00)
#define MSG_RPC_CALL      (0x01)
#define MSG_RPC_RESULT    (0x02)
#define MSG_IRQ           (0x03)
#define MSG_WAKEUP        (0x04)

typedef int signal_t;

typedef struct {
    byte message_no : 6; 
    dword source : 20;
    qword reserved : 38;
} recvmsg_result_struct;