// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/include/std/string.h
 *
 * 标准库string.h的头文件
 */



#pragma once

int strcmp(const char* str1, const char* str2); //比较两个字符串，相同返回0, str1 > str2 返回 1, str1 < str2 返回-1
int strlen(const char* str); //计算str长度
char* strcpy(char* dst, const char* src); //复制字符串src至字符串dst
int atoi(const char* str); //字符串转数字
int atoi_8(const char* str); //字符串转数字(8进制)
int atoi_16(const char* str); //字符串转数字(16进制)
unsigned int atoui(const char* str); //字符串转数字(无符号)
unsigned int atoui_8(const char* str); //字符串转数字(无符号)(8进制)
unsigned int atoui_16(const char* str); //字符串转数字(无符号)(16进制)
char* itoa(char* buffer, int value, int radix); //数字转字符串
char* uitoa(char* buffer, unsigned int value, int radix); //数字转字符串(无符号)
char* itoa_upper(char* buffer, int value, int radix); //数字转字符串(大写)
char* uitoa_upper(char* buffer, unsigned int value, int radix); //数字转字符串(无符号)(大写)
char* ftoa(char* buffer, float value); //不起效