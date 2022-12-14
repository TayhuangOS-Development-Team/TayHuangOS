/**
 * @file stddef.h
 * @author theflysong (song_of_the_fly@163.com) 383494
 * @brief 标准库头文件 stddef.h
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

#ifndef __SIZE_TYPE__
#define __SIZE_TYPE__ unsigned int
#endif

typedef __SIZE_TYPE__ size_t;

#ifndef __PTRDIFF_TYPE__

#ifndef LOADER32BIT
#define __PTRDIFF_TYPE__ signed long long
#else
#define __PTRDIFF_TYPE__ signed int
#endif

#endif

typedef __PTRDIFF_TYPE__ ptrdiff_t;

#ifndef offsetof
#define offsetof(type, member) ((size_t)&(((type *)0)->member))
#endif

#ifndef __cplusplus
typedef int wchar_t;
#define NULL ((void *)0)
#else
#define NULL 0
#endif

__C_HEADER_END;