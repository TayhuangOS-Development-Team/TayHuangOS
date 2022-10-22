/*
 * SPDX-License-Identifier: LGPL-2.1-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: 383494
 *
 * stdbool.h
 *
 * 标准库 stdbool.h 头文件
 *
 */

#pragma once

#ifndef _STDBOOL_H
#define _STDBOOL_H

#define __bool_true_false_are_defined 1

#ifndef __cplusplus
#define bool _Bool

#if (defined __STDC_VERSION__ && __STDC_VERSION__ > 201710L)	
// for C23
#define true ((_Bool) + 1u)
#define false ((_Bool) + 0u)
#else
#define true 1
#define false 0
#endif

#else
#define _Bool bool
#endif

#endif
