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
 * logging.c
 *
 * 日志
 *
 */



#include <debug/logging.h>
#include <tayhuang/ports.h>
#include <tayhuang/io.h>

#include <printf.h>

PRIVATE const char *logging_name;

PUBLIC void set_logging_name(const char *name) {
    logging_name = name;
}

PUBLIC void write_serial_char(char ch) {
    while ((inb(SERIAL_STATUS) & 0x20) == 0);
    outb(SERIAL_SEND, ch);
}

PUBLIC void write_serial_str(const char *str) {
    while (*str != '\0') {
        write_serial_char(*str);
        str ++;
    }
}

PUBLIC void __log(const char *type, const char *msg) {
    write_serial_char('(');
    write_serial_str(logging_name);
    write_serial_char(')');
    write_serial_char('[');
    write_serial_str(type);
    write_serial_char(']');
    write_serial_str(msg);
    write_serial_char('\n');
}

PUBLIC void _log(const int type, const char *fmt, va_list args) {
    const char *typename;
    switch (type) {
    case INFO: typename = "INFO"; break;
    case WARN: typename = "WARN"; break;
    case ERROR: typename = "ERROR"; break;
    case FATAL: typename = "FATAL"; break;
    case TIPS: typename = "TIPS"; break;
    case ATTENTION: typename = "ATTENTION"; break;
    default: typename = "UNKNOWN"; break;
    }

    char buffer[512];

    vsprintf(buffer, fmt, args);

    __log(typename, buffer);
}

PUBLIC void log(const char *type, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char buffer[512];
    vsprintf(buffer, fmt, args);

    __log(type, buffer);

    va_end(args);
}

PUBLIC void linfo(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    _log(INFO, fmt, args);

    va_end(args);
}

PUBLIC void lwarn(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    _log(WARN, fmt, args);

    va_end(args);
}

PUBLIC void lerror(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    _log(ERROR, fmt, args);

    va_end(args);
}

PUBLIC void lfatal(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    _log(FATAL, fmt, args);

    va_end(args);
}

PUBLIC void ltips(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    _log(TIPS, fmt, args);

    va_end(args);
}

PUBLIC void lattention(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    _log(ATTENTION, fmt, args);

    va_end(args);
}