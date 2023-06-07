/**
 * @file debug.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 调试
 * @version alpha-1.0.0
 * @date 2023-04-30
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#pragma once

#include <primlib/logger.h>

void init_serial(void);
void write_serial_char(char ch);
void write_serial_str(const char *str);