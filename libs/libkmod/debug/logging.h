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

PUBLIC void write_serial_char(char ch);
PUBLIC void write_serial_str(const char *str);
PUBLIC void set_logging_name(const char *name);

enum {
    INFO = 0,
    WARN,
    ERROR,
    FATAL,
    TIPS,
    ATTENTION
};

PUBLIC void log(const int type, const char *msg);
PUBLIC void linfo(const char *msg);
PUBLIC void lwarn(const char *msg);
PUBLIC void lerror(const char *msg);
PUBLIC void lfatal(const char *msg);
PUBLIC void ltips(const char *msg);
PUBLIC void lattention(const char *msg);