/**
 * @file assert.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief assert.h
 * @version alpha-1.0.0
 * @date 2022-12-31
 *
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#pragma once

/**
 * @brief 断言
 *
 * @param expression 表达式
 * @param file 文件
 * @param base_file 源文件
 * @param line 行
 */
void assertion_failure(const char *expression, const char *file, const char *base_file, int line);
/**
 * @brief 崩溃断言
 *
 * @param expression 表达式
 * @param file 文件
 * @param base_file 源文件
 * @param line 行
 */
void panic_failure(const char *expression, const char *file, const char *base_file, int line);
/**
 * @brief 崩溃
 *
 * @param format 格式化字符串
 * @param ... 可变参数
 */
void panic(const char *format, ...);

// 不开启DEBUG
#ifdef NDEBUG
    /** 断言 */
    #define assert(expression) ((void)0)
    /** 崩溃断言 */
    #define panic_assert(expression) ((void)0)
#else
    /** 断言 */
    #define assert(expression) if (! (expression)) \
        assertion_failure(#expression, __FILE__, __BASE_FILE__, __LINE__)
    /** 崩溃断言 */
    #define panic_assert(expression) if (! (expression)) \
        panic_failure(#expression, __FILE__, __BASE_FILE__, __LINE__)
#endif
