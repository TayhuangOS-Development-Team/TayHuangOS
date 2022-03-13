/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * math.h
 *
 * 标准库 math.h 头文件
 *
 */




#pragma once

#define HUGE_VAL (__builtin_huge_val())

#define CONSTANT_PI     (3.14159265358979323846) //圆周率 (why not acos -1???)
#define CONSTANT_E      (2.71828182845904523536) //e
#define CONSTANT_PHI    (0.61803398874989484820) //黄金分割比例
#define CONSTANT_DELTA  (4.66920160910299067185) //费根鲍姆常数

double sin(double x);
double cos(double x);
double atanh(double x);
double ln(double x);
double exp(double x);
double pow(double x, double y);
double sqrt(double x);
double fabs(double x);