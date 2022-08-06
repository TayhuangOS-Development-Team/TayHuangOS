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
 * syscall.h
 * 
 * 系统调用
 * 
 */



#pragma once

#include <tayhuang/defs.h>

PUBLIC void moo(void);
PUBLIC void reg_irq(int irq);
PUBLIC void exit(int ret);
PUBLIC bool test_and_lock(bool *val);

PUBLIC id_t create_signal(int max_signals, int value, bool soft);
PUBLIC int get_signals(id_t id);
PUBLIC void up_signal(id_t id);
PUBLIC void down_signal(id_t id);
PUBLIC bool is_soft_signal(id_t id);

typedef void(*thread_function)(void *);
PUBLIC int create_thread(thread_function entry, void *args);
PUBLIC void exit_thread(void *retval);
