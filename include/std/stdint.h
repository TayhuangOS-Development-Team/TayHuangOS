/**
 * @file stdint.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief stdint.h
 * @version alpha-1.0.0
 * @date 2022-12-31
 *
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#pragma once

#ifdef __INT8_TYPE__
/** 有符号8位整型 */
typedef __INT8_TYPE__ int8_t;
#endif

#ifdef __INT16_TYPE__
/** 有符号16位整型 */
typedef __INT16_TYPE__ int16_t;
#endif

#ifdef __INT32_TYPE__
/** 有符号32位整型 */
typedef __INT32_TYPE__ int32_t;
#endif

#ifdef __INT64_TYPE__
/** 有符号64位整型 */
typedef __INT64_TYPE__ int64_t;
#endif

#ifdef __UINT8_TYPE__
/** 无符号8位整型 */
typedef __UINT8_TYPE__ uint8_t;
#endif

#ifdef __UINT16_TYPE__
/** 无符号16位整型 */
typedef __UINT16_TYPE__ uint16_t;
#endif

#ifdef __UINT32_TYPE__
/** 无符号32位整型 */
typedef __UINT32_TYPE__ uint32_t;
#endif

#ifdef __UINT64_TYPE__
/** 无符号64位整型 */
typedef __UINT64_TYPE__ uint64_t;
#endif