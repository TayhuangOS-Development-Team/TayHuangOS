/**
 * @file logging.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 日志
 * @version alpha-1.0.0
 * @date 2022-10-23
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */


#pragma once

#include <tayhuang/attributes.h>
#include <stdarg.h>

/**
 * @brief 初始化串口
 * 
 */
PUBLIC void init_serial(void);
/**
 * @brief 向串口写字符
 * 
 */
PUBLIC void write_serial_char(char ch);
/**
 * @brief 向串口写字符串
 * 
 */
PUBLIC void write_serial_str(const char *str);

/**
 * @brief 日志类型
 * 
 */
enum {
    INFO = 0, //信息
    WARN,     //警告
    ERROR,    //错误
    FATAL     //致命错误
};

PUBLIC void __log(const char *position, const char *name, const char *type, const char *msg);
PUBLIC void _log(const char *file, int line, const char *name, const int type, const char *fmt, va_list args);
PUBLIC void log(const char *file, int line, const char *name, const char *typename, const char *fmt, ...);
PUBLIC void linfo(const char *file, int line, const char *name, const char *fmt, ...);
PUBLIC void lwarn(const char *file, int line, const char *name, const char *fmt, ...);
PUBLIC void lerror(const char *file, int line, const char *name, const char *fmt, ...);
PUBLIC void lfatal(const char *file, int line, const char *name, const char *fmt, ...);

#define LOGF(name, type, fmt, ...) log(__FILE__, __LINE__, name, type, fmt, __VA_ARGS__)
#define LINFOF(name, fmt, ...) linfo(__FILE__, __LINE__, name, fmt, __VA_ARGS__)
#define LWARNF(name, fmt, ...) lwarn(__FILE__, __LINE__, name, fmt, __VA_ARGS__)
#define LERRORF(name, fmt, ...) lerror(__FILE__, __LINE__, name, fmt, __VA_ARGS__)
#define LFATALF(name, fmt, ...) lfatal(__FILE__, __LINE__, name, fmt, __VA_ARGS__)

#define LOG(name, type, fmt) log(__FILE__, __LINE__, name, type, fmt)
#define LINFO(name, fmt) linfo(__FILE__, __LINE__, name, fmt)
#define LWARN(name, fmt) lwarn(__FILE__, __LINE__, name, fmt)
#define LERROR(name, fmt) lerror(__FILE__, __LINE__, name, fmt)
#define LFATAL(name, fmt) lfatal(__FILE__, __LINE__, name, fmt)