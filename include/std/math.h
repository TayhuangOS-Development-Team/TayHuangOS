/**
 * @file math.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief math.h
 * @version alpha-1.0.0
 * @date 2022-12-31
 *
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#pragma once

/** 巨大值 */
#define HUGE_VAL (__builtin_huge_val())

/** 圆周率 */
#define CONSTANT_PI     (3.14159265358979323846)
/** e */
#define CONSTANT_E      (2.71828182845904523536)
/** 黄金分割比例 */
#define CONSTANT_PHI    (0.61803398874989484820)
/** 费根鲍姆常数 */
#define CONSTANT_DELTA  (4.66920160910299067185)

double sin(double x); // 正弦
double cos(double x); // 余弦
double atanh(double x); // 反双曲正切
double ln(double x); // 自然对数
double exp(double x); // e^x
double pow(double x, double y); // x^y
double sqrt(double x); // 开方
double fabs(double x); // 绝对值