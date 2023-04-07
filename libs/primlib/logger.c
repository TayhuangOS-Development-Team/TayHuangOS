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

#include <primlib/logger.h>

void init_ulogger(prim_output_func output, const char *name) {
    output(name);
}