/**
 * @file tostring.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief tostring 头文件
 * @version alpha-1.0.0
 * @date 2023-04-09
 *
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#pragma once

/**
 * @brief 无符号整型转字符串
 *
 * @param val 值
 * @param buffer 缓存
 * @param base 进制
 * @return 字符串
 */
char *uitoa(unsigned int val, char *buffer, int base);

/**
 * @brief 整型转字符串
 *
 * @param val 值
 * @param buffer 缓存
 * @param base 进制
 * @return 字符串
 */
char *itoa(int val, char *buffer, int base);