/**
 * @file logger.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief logger 头文件
 * @version alpha-1.0.0
 * @date 2023-4-3
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#pragma once

#include <primlib/prim_io.h>
#include <stdarg.h>

/**
 * @brief 初始化 ulogger
 * 
 * @param output 输出函数
 * @param name 程序名
 */
void init_ulogger(prim_output_func output, const char *name);

/**
 * @brief 输出日志
 * 
 * @param name 日志名
 * @param level 日志等级
 * @param msg 日志消息
 */
void __ll_log__(const char *name, const char *level, const char *msg);

/**
 * @brief 日志等级枚举
 * 
 */
typedef enum {
    INFO = 0,
    WARNING,
    ERROR,
    FATAL
} log_level_t;

/**
 * @brief 输出日志
 * 
 * @param level 日志等级
 * @param msg 日志消息
 */
void __log__(log_level_t level, const char *msg);

/**
 * @brief 输出日志
 * 
 * @param level 日志等级
 * @param fmt 日志消息格式化字符串
 * @param args 日志消息参数
 */
void __logv__(log_level_t level, const char *fmt, va_list args);

/**
 * @brief 输出信息
 * 
 * @param fmt 日志消息格式化字符串
 */
void log_info(const char *fmt, ...);

/**
 * @brief 输出警告
 * 
 * @param fmt 日志消息格式化字符串
 */
void log_warning(const char *fmt, ...);

/**
 * @brief 输出错误
 * 
 * @param fmt 日志消息格式化字符串
 */
void log_error(const char *fmt, ...);

/**
 * @brief 输出致命错误
 * 
 * @param fmt 日志消息格式化字符串
 */
void log_fatal(const char *fmt, ...);