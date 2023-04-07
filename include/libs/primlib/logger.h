/**
 * @file logger.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief logger - API
 * @version alpha-1.0.0
 * @date 2023-4-3
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#pragma once

#include <primlib/prim_io.h>

/**
 * @brief 初始化 ulogger
 * 
 * @param output 输出函数
 * @param name 程序名
 */
void init_ulogger(prim_output_func output, const char *name);