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
 * logging.c
 *
 * 日志
 *
 */



#include <logging.h>
#include <tayhuang/ports.h>
#include <tayhuang/io.h>
#include <printk.h>

PUBLIC void init_serial(void) {
    outb(SERIAL_INT_VALID, 0); //禁用COM中断
    outb(SERIAL_CONTROL, 0x80); //启用DLAB
    outb(SERIAL_SEND, 0x03); //设置比特波除数(低)
    outb(SERIAL_INT_VALID, 0x00); //设置比特波除数(高)
    outb(SERIAL_CONTROL, 0x03); //无奇偶性 1停止位
    outb(SERIAL_INT_ID, 0xC7); //FIFO(size = 14)
    outb(SERIAL_MODEM_CONTROL, 0x0B);
    outb(SERIAL_MODEM_CONTROL, 0x1E);
    outb(SERIAL_SEND, 0xAE);
    outb(SERIAL_MODEM_CONTROL, 0x0F);
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

PUBLIC void __log(const char *name, const char *type, const char *msg) {
    write_serial_char('(');
    write_serial_str(name);
    write_serial_char(')');
    write_serial_char('[');
    write_serial_str(type);
    write_serial_char(']');
    write_serial_str(msg);
    write_serial_char('\n');
}

PUBLIC void _log(const char *name, const int type, const char *fmt, va_list args) {
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

    vsprintk(buffer, fmt, args);

    __log(name, typename, buffer);
}

PUBLIC void log(const char *name, const char *type, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char buffer[512];
    vsprintk(buffer, fmt, args);

    __log(name, type, buffer);

    va_end(args);
}

PUBLIC void linfo(const char *name, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    _log(name, INFO, fmt, args);

    va_end(args);
}

PUBLIC void lwarn(const char *name, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    _log(name, WARN, fmt, args);

    va_end(args);
}

PUBLIC void lerror(const char *name, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    _log(name, ERROR, fmt, args);

    va_end(args);
}

PUBLIC void lfatal(const char *name, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    _log(name, FATAL, fmt, args);

    va_end(args);
}

PUBLIC void ltips(const char *name, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    _log(name, TIPS, fmt, args);

    va_end(args);
}

PUBLIC void lattention(const char *name, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    _log(name, ATTENTION, fmt, args);

    va_end(args);
}