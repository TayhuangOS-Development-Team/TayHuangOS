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

struct __msgpack_struct {
    byte reserved : 8;
    byte message_no : 8;

    word msg_id : 16;
    word length : 16; 
    word source : 16;
} __attribute__((packed));

struct __msgno_id {
    byte reserved : 8;
    byte message_no : 8;
    word msg_id : 16;

} __attribute__((packed));

typedef struct __msgno_id msgno_id;
typedef struct __msgpack_struct msgpack_struct;

#define MSG_NORMAL_IPC    (0x00)
#define MSG_RPC_CALL      (0x01)
#define MSG_RPC_RESULT    (0x02)
#define MSG_IRQ           (0x03)
#define MSG_WAKEUP        (0x04)