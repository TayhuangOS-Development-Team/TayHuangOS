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



#include <logging.h>

#include <tayhuang/ports.h>
#include <tayhuang/io.h>

PUBLIC void init_serial(void) {
    outb(SERIAL_INT_VALID, 0); //禁用COM中断
    outb(SERIAL_CONTROL, 0x80); //启用DLAB
    outb(SERIAL_SEND, 0x03); //设置比特波除数(低)
    outb(SERIAL_INT_VALID, 0x00); //设置比特波除数(高)
    outb(SERIAL_CONTROL, 0x03); //无奇偶性 1停止位
    outb(SERIAL_INT_ID, 0xC7); //FIFO(size = 14)
    outb(SERIAL_MODEM_CONTROL, 0x0B); //
    outb(SERIAL_MODEM_CONTROL, 0x1E); //
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

PUBLIC void _log(const char *name, const char *type, const char *msg) {
    asmv ("cli");
    write_serial_char('(');
    write_serial_str(name);
    write_serial_char(')');
    write_serial_char('[');
    write_serial_str(type);
    write_serial_char(']');
    write_serial_str(msg);
    write_serial_char('\n');
    asmv ("sti");
}

PUBLIC void log(const char *name, const int type, const char *msg) {
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
    _log(name, typename, msg);
}

PUBLIC void linfo(const char *name, const char *msg) {
    log(name, INFO, msg);
}

PUBLIC void lwarn(const char *name, const char *msg) {
    log(name, WARN, msg);
}

PUBLIC void lerror(const char *name, const char *msg) {
    log(name, ERROR, msg);
}

PUBLIC void lfatal(const char *name, const char *msg) {
    log(name, FATAL, msg);
}

PUBLIC void ltips(const char *name, const char *msg) {
    log(name, TIPS, msg);
}

PUBLIC void lattention(const char *name, const char *msg) {
    log(name, ATTENTION, msg);
}