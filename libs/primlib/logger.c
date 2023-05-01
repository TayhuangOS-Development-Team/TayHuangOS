/**
 * @file logger.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief logger - 实现
 * @version alpha-1.0.0
 * @date 2023-4-3
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <primlib/logger.h>
#include <primlib/prim_io.h>
#include <stddef.h>

// 日志名
static const char *logger_name = NULL;

// 输出流
static prim_output_func ostream;

void init_ulogger(prim_output_func output, const char *name) {
    pprintf(output, "[ULogger/INFO]Initializing Logger for [%s].\n", name);

    // 初始化

    logger_name = name;
    ostream = output;
}

void __ll_log__(const char *name, const char *level, const char *msg) {
    pprintf(ostream, "[%s/%s]%s\n", name, level, msg);
}

void __log__(log_level_t level, const char *msg) {
    // 日志等级字符串
    const char *level_s = "";

    // 获取日志等级
    switch(level) {
    case INFO:    {level_s = "INFO";    break;}
    case WARNING: {level_s = "WARNING"; break;}
    case ERROR:   {level_s = "ERROR";   break;}
    case FATAL:   {level_s = "FATAL";   break;}
    }

    __ll_log__(logger_name, level_s, msg);
}

void __logv__(log_level_t level, const char *fmt, va_list args) {
    char buffer[512];

    vsprintf(buffer, fmt, args);

    __log__(level, buffer);
}

void log_info(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    __logv__(INFO, fmt, args);

    va_end(args);
}

void log_warning(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    __logv__(WARNING, fmt, args);

    va_end(args);
}

void log_error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    __logv__(ERROR, fmt, args);

    va_end(args);
}

void log_fatal(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    __logv__(FATAL, fmt, args);

    va_end(args);
}