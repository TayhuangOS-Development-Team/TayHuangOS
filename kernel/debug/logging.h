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
 * logging.h
 *
 * 日志
 *
 */



#pragma once

#include <tayhuang/defs.h>

#include <intterup/init_int.h>

PUBLIC void init_serial(void);
PUBLIC void write_serial_char(char ch);
PUBLIC void write_serial_str(const char *str);

enum {
    INFO = 0,
    WARN,
    ERROR,
    FATAL,
    TIPS,
    ATTENTION
};

PUBLIC void _log(const char *type, const char *msg);
PUBLIC void log(const int type, const char *msg);
PUBLIC void linfo(const char *msg);
PUBLIC void lwarn(const char *msg);
PUBLIC void lerror(const char *msg);
PUBLIC void lfatal(const char *msg);
PUBLIC void ltips(const char *msg);
PUBLIC void lattention(const char *msg);

PUBLIC short serial_irq_handler(int irq, struct intterup_args *regs, bool entered_handler);