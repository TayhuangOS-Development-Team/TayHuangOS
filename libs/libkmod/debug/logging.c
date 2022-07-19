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
    while ((inb(SERIAL_STATUS) & 0x20) == 0); //可以使用
    outb(SERIAL_SEND, ch); //输出一个字符
}

PUBLIC void write_serial_str(const char *str) {
    while (*str != '\0') { //不遇到\0就不停下
        write_serial_char(*str);
        str ++;
    }
}

PUBLIC void __log(const char *type, const char *msg) {
    //打印名字
    write_serial_char('(');
    write_serial_str(logging_name);
    write_serial_char(')');
    //打印类型
    write_serial_char('[');
    write_serial_str(type);
    write_serial_char(']');
    //输出消息
    write_serial_str(msg);
    //换行
    write_serial_char('\n');
}

PUBLIC void _log(const int type, const char *fmt, va_list args) {
    const char *typename;
    
    //根据类型获取类型名
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

    //输出到buffer里
    vsprintf(buffer, fmt, args);

    //输出到串口
    __log(typename, buffer);
}

PUBLIC void log(const char *type, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    //输出到buffer里
    char buffer[512];
    vsprintf(buffer, fmt, args);

    //输出到串口
    __log(type, buffer);

    va_end(args);
}

//信息
PUBLIC void linfo(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    _log(INFO, fmt, args);

    va_end(args);
}

//警告
PUBLIC void lwarn(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    _log(WARN, fmt, args);

    va_end(args);
}

//错误
PUBLIC void lerror(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    _log(ERROR, fmt, args);

    va_end(args);
}

//致命错误
PUBLIC void lfatal(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    _log(FATAL, fmt, args);

    va_end(args);
}

//提示
PUBLIC void ltips(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    _log(TIPS, fmt, args);

    va_end(args);
}

//注意
PUBLIC void lattention(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    _log(ATTENTION, fmt, args);

    va_end(args);
}