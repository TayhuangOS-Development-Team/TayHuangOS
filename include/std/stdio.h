/**
 * @file stdio.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief stdio.h
 * @version alpha-1.0.0
 * @date 2023-08-21
 *
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#pragma once

/**
 * @brief 输出到buffer中
 *
 * @param buffer 缓存
 * @param fmt 格式化字符串
 * @param args 参数
 * @return 输出字符数
 */
int vsprintf(char *buffer, const char *fmt, va_list args);

/**
 * @brief 输出到buffer中
 *
 * @param buffer 缓存
 * @param fmt 格式化字符串
 * @param ... 参数
 * @return 输出字符数
 */
int sprintf(char *buffer, const char *fmt, ...);