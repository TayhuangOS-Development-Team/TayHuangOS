/**
 * @file printf.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 打印
 * @version alpha-1.0.0
 * @date 2022-10-23
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#pragma once

#include <tayhuang/attributes.h>
#include <tayhuang/types.h>
#include <stdarg.h>

/**
 * @brief 打印到缓存
 * 最基础的printf
 * 
 * @param buffer 缓存
 * @param format format字符串
 * @param args format参数
 * @return 打印的字符个数 
 */
PUBLIC int vsprintf(char *buffer, const char *format, va_list args);
/**
 * @brief 打印到缓存
 * 
 * @param buffer 缓存
 * @param format format字符串
 * @param ... format参数
 * @return 打印的字符个数 
 */
PUBLIC int sprintf(char *buffer, const char *format, ...);