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

#include <tool/io.h>

//SERIAL
#define SERIAL_BASE          (0x3F8)
#define SERIAL_SEND          (SERIAL_BASE + 0)
#define SERIAL_KEEP          (SERIAL_BASE + 0)
#define SERIAL_INT_VALID     (SERIAL_BASE + 1)
#define SERIAL_INT_ID        (SERIAL_BASE + 2)
#define SERIAL_CONTROL       (SERIAL_BASE + 3)
#define SERIAL_MODEM_CONTROL (SERIAL_BASE + 4)
#define SERIAL_STATUS        (SERIAL_BASE + 5)
#define SERIAL_MODEM_STATUS  (SERIAL_BASE + 6)

void write_serial_char(char ch) {
    while ((inb(SERIAL_STATUS) & 0x20) == 0);
    outb(SERIAL_SEND, ch);
}

void write_serial_str(const char *str) {
    while (*str != '\0') {
        write_serial_char(*str);
        str ++;
    }
}

static const char *logging_name = NULL;

void set_logging_name(const char *name) {
    logging_name = name;
}

static void _log(const char *type, const char *msg) {
    asmv ("cli");
    if (logging_name != NULL) {
        write_serial_char('(');
        write_serial_str(logging_name);
        write_serial_char(')');
    }
    write_serial_char('[');
    write_serial_str(type);
    write_serial_char(']');
    write_serial_str(msg);
    write_serial_char('\n');
    asmv ("sti");
}

void log(const int type, const char *msg) {
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
    _log(typename, msg);
}

void linfo(const char *msg) {
    log(INFO, msg);
}

void lwarn(const char *msg) {
    log(WARN, msg);
}

void lerror(const char *msg) {
    log(ERROR, msg);
}

void lfatal(const char *msg) {
    log(FATAL, msg);
}

void ltips(const char *msg) {
    log(TIPS, msg);
}

void lattention(const char *msg) {
    log(ATTENTION, msg);
}