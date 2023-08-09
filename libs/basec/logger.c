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

#include <basec/logger.h>
#include <basec/baseio.h>
#include <stddef.h>

// 日志名
static const char *loggerName = NULL;

// 输出流
static BaseCPutsFunc loggerPuts;

/**
 * @brief 初始化 ulogger
 * 
 * @param bputs 输出函数
 * @param name 程序名
 */
void InitLogger(BaseCPutsFunc bputs, const char *name) {
    bprintf(bputs, "[BCL Logger/INFO]为[%s]初始化日志器中.\n", name);

    // 初始化
    loggerName = name;
    loggerPuts = bputs;
}

/**
 * @brief 输出日志
 * 
 * @param name 日志名
 * @param level 日志等级
 * @param msg 日志消息
 */
void __LLog__(const char *name, const char *level, const char *msg) {
    bprintf(loggerPuts, "[%s/%s]%s\n", name, level, msg);
}

/**
 * @brief 输出日志
 * 
 * @param level 日志等级
 * @param msg 日志消息
 */
void __Log__(LogLevel level, const char *msg) {
    // 日志等级字符串
    const char *level_s = "";

    // 获取日志等级
    switch(level) {
    case INFO:    {level_s = "INFO";    break;}
    case WARNING: {level_s = "WARNING"; break;}
    case ERROR:   {level_s = "ERROR";   break;}
    case FATAL:   {level_s = "FATAL";   break;}
    }

    __LLog__(loggerName, level_s, msg);
}

/**
 * @brief 输出日志
 * 
 * @param level 日志等级
 * @param fmt 日志消息格式化字符串
 * @param args 日志消息参数
 */
void __LogV__(LogLevel level, const char *fmt, va_list args) {
    char buffer[512];

    vsprintf(buffer, fmt, args);

    __Log__(level, buffer);
}

/**
 * @brief 输出信息
 * 
 * @param fmt 日志消息格式化字符串
 */
void LogInfo(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    __LogV__(INFO, fmt, args);

    va_end(args);
}

/**
 * @brief 输出警告
 * 
 * @param fmt 日志消息格式化字符串
 */
void LogWarn(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    __LogV__(WARNING, fmt, args);

    va_end(args);
}

/**
 * @brief 输出错误
 * 
 * @param fmt 日志消息格式化字符串
 */
void LogError(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    __LogV__(ERROR, fmt, args);

    va_end(args);
}

/**
 * @brief 输出致命错误
 * 
 * @param fmt 日志消息格式化字符串
 */
void LogFatal(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    __LogV__(FATAL, fmt, args);

    va_end(args);
}