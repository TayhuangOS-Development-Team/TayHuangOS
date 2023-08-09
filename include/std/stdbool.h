/**
 * @file stdbool.h
 * @author 383494
 * @brief stdbool
 * @version alpha-1.0.0
 * @date 2022-12-31
 *
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#pragma once

#ifndef _STDBOOL_H
#define _STDBOOL_H

/** 是否定义布尔 */
#define __bool_true_false_are_defined 1

#ifndef __cplusplus
/** 布尔 */
#define bool _Bool

#if (defined __STDC_VERSION__ && __STDC_VERSION__ > 201710L)
// for C23
/** 真 */
#define true ((_Bool) + 1u)
/** 假 */
#define false ((_Bool) + 0u)
#else
/** 真 */
#define true 1
/** 假 */
#define false 0
#endif

#else
/** 布尔 */
#define _Bool bool
#endif

#endif
