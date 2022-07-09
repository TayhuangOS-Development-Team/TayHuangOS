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

#define GET_READ_PTR_SN (0x03)
PUBLIC void *__get_read_ptr(void);
PUBLIC void *get_read_ptr(void);

#define SET_READ_PTR_SN (0x04)
PUBLIC void __set_read_ptr(void *read_ptr);
PUBLIC void set_read_ptr(void *read_ptr);

#define GET_USED_SIZE_SN (0x05)
PUBLIC qword __get_used_size(void);
PUBLIC qword get_used_size(void);

#define SET_USED_SIZE_SN (0x06)
PUBLIC void __set_used_size(qword used_size);
PUBLIC void set_used_size(qword used_size);

#define SETMAIL_BUFFER_SN (0x07)
PUBLIC void __setmail_buffer(void *buffer, qword size);
PUBLIC void setmail_buffer(void *buffer, qword size);

PUBLIC bool recvmsg(void *dst, int src);
PUBLIC int recvanymsg(void *dst);