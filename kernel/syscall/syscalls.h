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
 * syscalls.h
 * 
 * 系统调用的实现
 * 
 */



#pragma once

#include <tayhuang/defs.h>
#include <tayhuang/msgpack.h>

#include <intterup/irq_handler.h>

//超级牛力
#define MOO_SN (0x00)
PUBLIC void __moo(void);
PUBLIC void moo(void);

//IPC相关
#define SEND_MSG_SN (0x01)
PUBLIC bool __send_msg(int msgno, void *src, qword size, int dst);
PUBLIC bool send_msg(int msgno, void *src, qword size, int dst);

#define CHECK_IPC_SN (0x02)
PUBLIC void __check_ipc(void);
PUBLIC void check_ipc(void);

#define SET_ALLOW_SN (0x03)
PUBLIC void __set_allow(int pid);
PUBLIC void set_allow(int pid);

#define RECV_MSG_SN (0x04)
PUBLIC recvmsg_result_struct __recv_msg(void *dst);
PUBLIC recvmsg_result_struct recv_msg(void *dst);

#define CLEAR_MAIL_SN (0x05)
PUBLIC void __clear_mail(void);
PUBLIC void clear_mail(void);

//锁
#define TEST_AND_LOCK_SN (0x06)
PUBLIC bool __test_and_lock(bool *val);
PUBLIC bool test_and_lock(bool *val);

#define SET_MAILBUFFER_SN (0x07)
PUBLIC void __set_mailbuffer(void *buffer, qword size);
PUBLIC void set_mailbuffer(void *buffer, qword size);

#define CREATE_SIGNAL_SN (0x08)
PUBLIC id_t create_signal(int max_signals, int value, bool soft);

#define GET_SIGNALS_SN (0x09)
PUBLIC int get_signals(id_t id);

#define UP_SIGNAL_SN (0x0A)
PUBLIC void up_signal(id_t id);

#define DOWN_SIGNAL_SN (0x0B)
PUBLIC void down_signal(id_t id);

#define IS_SOFT_SIGNAL_SN (0x0C)
PUBLIC bool is_soft_signal(id_t id);

//其他
#define REG_IRQ_SN (0x10)
PUBLIC void __reg_irq(int irq);
PUBLIC void reg_irq(int irq);

typedef void(*thread_function)(void *);

//线程
#define CREATE_THREAD_SN (0x20)
PUBLIC int __create_thread(thread_function entry, void *args);
PUBLIC int create_thread(thread_function entry, void *args);

#define EXIT_THREAD_SN (0x21)
PUBLIC void __exit_thread(void *retval);
PUBLIC void exit_thread(void *retval);

//IRQ处理器
PUBLIC void normal_irq_handler(int irq, struct intterup_args *args, bool flags);

//dummy进程发送消息
PUBLIC bool dummy_send_msg(int msgno, void *src, qword size, int dst);