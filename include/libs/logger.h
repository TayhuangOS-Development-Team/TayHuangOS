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

typedef void(*ulogger_output_t)(const char *);

void init_ulogger(ulogger_output_t output, const char *name);