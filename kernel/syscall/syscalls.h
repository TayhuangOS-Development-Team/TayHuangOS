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

#define SENDMSG_SN (0x01)
PUBLIC bool __sendmsg(void *src, qword size, int dst);
PUBLIC bool sendmsg(void *src, qword size, int dst);

#define WAIT_IPC_SN (0x02)
PUBLIC void __wait_ipc(int pid);
PUBLIC void wait_ipc(int pid);

#define SETMAIL_BUFFER_SN (0x04)
PUBLIC void __setmail_buffer(void *buffer, qword size);
PUBLIC void setmail_buffer(void *buffer, qword size);