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

#include <tayhuang/types.h>

bool init_serial(void);
void write_serial_char(char ch);
void write_serial_str(const char *str);
void set_logging_name(const char *name);

enum {
    INFO = 0,
    WARN,
    ERROR,
    FATAL,
    TIPS,
    ATTENTION
};

void log(const int type, const char *msg);
void linfo(const char *msg);
void lwarn(const char *msg);
void lerror(const char *msg);
void lfatal(const char *msg);
void ltips(const char *msg);
void lattention(const char *msg);