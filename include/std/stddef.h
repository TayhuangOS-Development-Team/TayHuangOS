/**
 * @file stddef.h
 * @author theflysong (song_of_the_fly@163.com) 383494
 * @brief stddef.h
 * @version alpha-1.0.0
 * @date 2022-12-31
 *
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#pragma once

/** 空指针 */
#define NULL ((void *)0)

#ifndef __SIZE_TYPE__
/** size的类型 */
#define __SIZE_TYPE__ unsigned int
#endif

/** size的类型 */
typedef __SIZE_TYPE__ size_t;

#ifndef __PTRDIFF_TYPE__

#ifndef LOADER32BIT
/** 指针差值类型 */
#define __PTRDIFF_TYPE__ signed long long
#else
/** 指针差值类型 */
#define __PTRDIFF_TYPE__ signed int
#endif

#endif

/** 指针差值类型 */
typedef __PTRDIFF_TYPE__ ptrdiff_t;

#ifndef offsetof
/** 取偏移 */
#define offsetof(type, member) ((size_t)&(((type *)0)->member))
#endif


#ifndef __WCHAR_TYPE__
/** 宽字符 */
#define __WCHAR_TYPE__ int
#endif
#ifndef __cplusplus
/** 宽字符 */
typedef __WCHAR_TYPE__ wchar_t;
#endif
