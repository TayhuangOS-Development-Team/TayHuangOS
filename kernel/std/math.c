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
 * math.c
 *
 * 标准库 math.h 源文件
 *
 */

#include <math.h>

double sin(double x)
{
    return x - ((x * x * x) / 6.0) + ((x * x * x * x * x) / 120.0) - ((x * x * x * x * x * x * x) / 5040.0) + ((x * x * x * x * x * x * x * x * x) / 362880.0) - ((x * x * x * x * x * x * x * x * x * x * x) / 39916800.0) + ((x * x * x * x * x * x * x * x * x * x * x * x * x) / 6227020800.0) - ((x * x * x * x * x * x * x * x * x * x * x * x * x * x * x) / 1307674368000.0);
}

double cos(double x)
{
    return 1 - ((x * x) / 2.0) + ((x * x * x * x) / 24.0) - ((x * x * x * x * x * x) / 720.0) + ((x * x * x * x * x * x * x * x) / 40320.0) - ((x * x * x * x * x * x * x * x * x * x) / 3628800.0) + ((x * x * x * x * x * x * x * x * x * x * x * x) / 3628800.0) - ((x * x * x * x * x * x * x * x * x * x * x * x * x * x) / 87178291200.0);
}

double atanh(double x)
{
    return x + ((x * x * x) / 6.0) + ((x * x * x * x * x) / 120.0) + ((x * x * x * x * x * x * x) / 5040.0) + ((x * x * x * x * x * x * x * x * x) / 362880.0) + ((x * x * x * x * x * x * x * x * x * x * x) / 39916800.0) + ((x * x * x * x * x * x * x * x * x * x * x * x * x) / 6227020800.0) + ((x * x * x * x * x * x * x * x * x * x * x * x * x * x * x) / 1307674368000.0);
}

double ln(double x)
{
    return 2 * atanh((x - 1) / (x + 1));
}

double exp(double x)
{
    return 1 + x + (x * x) / 2.0 + (x * x * x) / 6.0 + (x * x * x * x) / 24.0 + (x * x * x * x * x) / 120.0 + (x * x * x * x * x * x) / 720.0 + (x * x * x * x * x * x * x) / 5040.0;
}

double pow(double x, double y)
{
    return exp((ln(x) * y));
}

double sqrt(double x)
{
    union{
        int intPart;
        float floatPart;
    } convertor;
    union{
        int intPart;
        float floatPart;
    } convertor2;
    convertor.floatPart = x;
    convertor2.floatPart = x;
    convertor.intPart = 0x1FBCF800 + (convertor.intPart >> 1);
    convertor2.intPart = 0x5f3759df - (convertor2.intPart >> 1);
    return 0.5f*(convertor.floatPart + (x * convertor2.floatPart)); //From Quake III
}

double fabs(double x)
{
    return x > 0 ? x : -x;
}