/**
 * @file __header.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief header
 * @version alpha-1.0.0
 * @date 2022-12-14
 *
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#pragma once

/**
 * @brief 声明
 *
 */
#define EXTERN extern

/**
 * @brief C头
 *
 */
#ifdef __cplusplus
#define __C_HEADER_START extern "C" {
#define __C_HEADER_END }
#define EXPORT extern "C"
#else
#define __C_HEADER_START
#define __C_HEADER_END
#define EXPORT
#endif