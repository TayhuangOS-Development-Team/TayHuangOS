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
 * logging.h
 *
 * 日志
 *
 */



#pragma once

#include <tayhuang/defs.h>

PUBLIC void init_serial(void);
PUBLIC void write_serial_char(char ch);
PUBLIC void write_serial_str(NONNULL const char *str);

enum {
    INFO = 0,
    WARN,
    ERROR,
    FATAL,
    TIPS,
    ATTENTION
};

AUTOMATIC PUBLIC void _log(NONNULL const char *name, NONNULL const char *type, NONNULL const char *msg);
PUBLIC void log(NONNULL const char *name, const int type, NONNULL const char *msg);
PUBLIC void linfo(NONNULL const char *name, NONNULL const char *msg);
PUBLIC void lwarn(NONNULL const char *name, NONNULL const char *msg);
PUBLIC void lerror(NONNULL const char *name, NONNULL const char *msg);
PUBLIC void lfatal(NONNULL const char *name, NONNULL const char *msg);
PUBLIC void ltips(NONNULL const char *name, NONNULL const char *msg);
PUBLIC void lattention(NONNULL const char *name, NONNULL const char *msg);