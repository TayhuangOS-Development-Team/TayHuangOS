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
    qword length : 38;
    byte message_no : 6; 
    dword source : 20;
} msgpack_struct;

#define MSG_NORMAL_IPC (0x00)
#define MSG_RPC_CALL   (0x01)
#define MSG_RPC_RESULT (0x02)
#define MSG_IRQ_WAKE   (0x03)

typedef struct {
    qword reserved : 38;
    byte message_no : 6; 
    dword source : 20;
} recvmsg_result_struct;