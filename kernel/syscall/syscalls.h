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
#include <intterup/irq_handler.h>

#define MOO_SN (0x00)
PUBLIC void __moo(void);
PUBLIC void moo(void);

#define SEND_MSG_SN (0x01)
PUBLIC bool __send_msg(void *src, qword size, int dst);
PUBLIC bool send_msg(void *src, qword size, int dst);

#define CHECK_IPC_SN (0x02)
PUBLIC void __check_ipc(void);
PUBLIC void check_ipc(void);

#define SET_ALLOW_SN (0x03)
PUBLIC void __set_allow(int pid);
PUBLIC void set_allow(int pid);

#define RECV_MSG_SN (0x04)
PUBLIC int __recv_msg(void *dst);
PUBLIC int recv_msg(void *dst);

#define SET_MAILBUFFER_SN (0x07)
PUBLIC void __set_mailbuffer(void *buffer, qword size);
PUBLIC void set_mailbuffer(void *buffer, qword size);

#define REG_IRQ_SN (0x0A)
PUBLIC void __reg_irq(int irq);
PUBLIC void reg_irq(int irq);

PUBLIC void normal_irq_handler(int irq, struct intterup_args *args, bool flags);
PUBLIC bool dummy_send_msg(void *src, qword size, int dst);