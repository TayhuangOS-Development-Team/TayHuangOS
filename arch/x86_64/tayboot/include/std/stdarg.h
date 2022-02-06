/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * stdarg.h
 *
 * 标准库 stdarg.h 头文件
 * 参考自linux
 *
 */



#pragma once

//定义类型
typedef char *va_list;

//int的size
#define INTSIZE (sizeof(int))
//将size转为INTSIZE的整数倍
#define INTSIZEOF(ty) ((sizeof(ty) + INTSIZE - 1) & (~(INTSIZE - 1)))

//初始化可变参数列表
#define va_start(lst, start) (lst = (((va_list)&start) + INTSIZEOF(start)))
//获取列表参数
#define va_arg(lst, ty) (*(ty*)((lst += INTSIZEOF(ty)) - INTSIZEOF(ty)))
//结束获取
#define va_end(lst) (lst = ((va_list)0))