/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: theflysong
 *
 * stddef.h
 *
 * 标准库 stddef.h 头文件
 *
 */



#pragma once

#define NULL ((void*)0)

#ifndef __SIZE_TYPE__
#define __SIZE_TYPE__ unsigned int
#endif

typedef __SIZE_TYPE__ size_t;

#ifndef __PTRDIFF_TYPE__

#ifndef LOADER32BIT
#define __PTRDIFF_TYPE__ unsigned long long
#else
#define __PTRDIFF_TYPE__ unsigned int
#endif

#endif

typedef __PTRDIFF_TYPE__ ptrdiff_t;