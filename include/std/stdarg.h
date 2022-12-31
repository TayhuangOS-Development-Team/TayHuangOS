/**
 * @file stdarg.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief stdarg.h 参考自linux
 * @version alpha-1.0.0
 * @date 2022-12-31
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#pragma once


#ifdef ARCH_x86_64

#ifndef __GNUC_VA_LIST
#define __GNUC_VA_LIST
typedef __builtin_va_list __gnuc_va_list;
#endif

#ifndef __va_list__
typedef __gnuc_va_list va_list;
#endif

#define va_start(v,l)	__builtin_va_start(v,l)
#define va_end(v)	__builtin_va_end(v)
#define va_arg(v,l)	__builtin_va_arg(v,l)
#define va_copy(d,s)	__builtin_va_copy(d,s)

#else

//int的size
#define INTSIZE (sizeof(int))

//定义类型
typedef char *va_list;

//将size转为INTSIZE的整数倍
#define INTSIZEOF(ty) ((sizeof(ty) + INTSIZE - 1) & (~(INTSIZE - 1)))

//初始化可变参数列表
#define va_start(lst, start) (lst = (((va_list)&start) + INTSIZEOF(start)))
//获取列表参数
#define va_arg(lst, ty) (*(ty *)((lst += INTSIZEOF(ty)) - INTSIZEOF(ty)))
//结束获取
#define va_end(lst) (lst = ((va_list)0))

#endif