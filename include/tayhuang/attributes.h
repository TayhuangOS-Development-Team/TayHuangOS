/**
 * @file attributes.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 属性
 * @version alpha-1.0.0
 * @date 2022-10-22
 *
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#pragma once

#include <__header.h>

__C_HEADER_START;

/**
 * @brief 公共的
 *
 */
#define PUBLIC
/**
 * @brief API函数声明
 *
 */
#define API extern
/**
 * @brief API函数实现
 *
 */
#define IMPL
/**
 * @brief 私有的
 *
 */
#define PRIVATE static
/**
 * @brief 易变的
 *
 */
#define VOLATILE volatile
/**
 * @brief 不返回的
 *
 */
#define NORETURN
/**
 * @brief 不可为空的
 *
 */
#define NONNULL
/**
 * @brief 可为空的
 *
 */
#define NULLABLE
/**
 * @brief 不可到达的
 *
 */
#define UNREACHABLE
/**
 * @brief 原子操作
 *
 */
#define AUTOMATIC
/**
 * @brief 内连函数
 *
 */
#define INLINE static inline
/**
 * @brief 由汇编实现
 *
 */
#define ASSEMBLY
/**
 * @brief 某个架构特有的
 *
 */
#define ARCHITECTURE_BASED
/**
 * @brief this指针
 *
 */
#define THIS
/**
 * @brief 构造函数
 *
 */
#define CONSTRUCTOR
/**
 * @brief 析构函数
 *
 */
#define DECONSTRUCTOR

__C_HEADER_END;