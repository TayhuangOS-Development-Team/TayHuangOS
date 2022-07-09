/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 * 
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 * 
 * --------------------------------------------------------------------------------
 * 
 * 作者: theflysong
 * 
 * syscalls.h
 * 
 * 系统调用的实现
 * 
 */



#pragma once

#include <tayhuang/defs.h>

#define MOO_SN (0x00)
PUBLIC void __moo(void);
PUBLIC void moo(void);

#define SEND_MSG_SN (0x01)
PUBLIC bool __send_msg(void *src, qword size, int dst);
PUBLIC bool send_msg(void *src, qword size, int dst);

#define CHECK_IPC_SN (0x02)
PUBLIC void __check_ipc(void);
PUBLIC void check_ipc(void);

#define RECV_MSG_SN (0x04)
PUBLIC int __recv_msg(void *dst);
PUBLIC int recv_msg(void *dst);

#define SET_MAILBUFFER_SN (0x07)
PUBLIC void __set_mailbuffer(void *buffer, qword size);
PUBLIC void set_mailbuffer(void *buffer, qword size);
