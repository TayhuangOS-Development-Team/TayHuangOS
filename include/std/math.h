/**
 * @file math.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 标准库头文件math.h
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

#define HUGE_VAL (__builtin_huge_val())

#define CONSTANT_PI     (3.14159265358979323846) //圆周率
#define CONSTANT_E      (2.71828182845904523536) //e
#define CONSTANT_PHI    (0.61803398874989484820) //黄金分割比例
#define CONSTANT_DELTA  (4.66920160910299067185) //费根鲍姆常数

EXPORT double sin(double x);
EXPORT double cos(double x);
EXPORT double atanh(double x);
EXPORT double ln(double x);
EXPORT double exp(double x);
EXPORT double pow(double x, double y);
EXPORT double sqrt(double x);
EXPORT double fabs(double x);

__C_HEADER_END;