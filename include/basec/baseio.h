/**
 * @file baseio.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief primitive io 头文件
 * @version alpha-1.0.0
 * @date 2023-04-08
 *
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#pragma once

#include <stdarg.h>
#include <stdio.h>

/**
 * @brief 输出函数
 */
typedef void(*BaseCPutsFunc)(const char *);

/**
 * @brief 输入函数
 */
typedef char(*BaseCGetcharFunc)(void);

/**
 * @brief 使用bputs输出
 *
 * @param bputs 输出函数
 * @param fmt 格式化字符串
 * @param args 参数
 * @return 输出字符数
 */
int vbprintf(BaseCPutsFunc bputs, const char *fmt, va_list args);

/**
 * @brief 使用bputs输出
 *
 * @param bputs 输出函数
 * @param fmt 格式化字符串
 * @param ... 参数
 * @return 输出字符数
 */
int bprintf(BaseCPutsFunc bputs, const char *fmt, ...);

/**
 * @brief 使用bgetchar输入
 *
 * @param bgetchar 输入函数
 * @param fmt 格式化字符串
 * @param args 参数
 * @return 输入字符数
 */
int vbscanf(BaseCGetcharFunc bgetchar, const char *fmt, va_list args);

/**
 * @brief 使用bgetchar输入
 *
 * @param bgetchar 输入函数
 * @param fmt 格式化字符串
 * @param ... 参数
 * @return 输入字符数
 */
int bscanf(BaseCGetcharFunc bgetchar, const char *fmt, ...);