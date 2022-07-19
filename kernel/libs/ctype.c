/* 
 * SPDX-License-Identifier: LGPL-2.1-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: theflysong
 *
 * ctype.h
 *
 * 标准库ctype实现
 *
 */

#include <ctype.h>

//利用提供好的宏实现
//目前只支持ascii字符

//空字符
int isspace(int ch) {
    return __isspace(ch);
}

//大写字母
int isupper(int ch) {
    return __isupper(ch);
}

//小写字母
int islower(int ch) {
    return __islower(ch);
}

//字母
int isalpha(int ch) {
    return __isalpha(ch);
}

//数字
int isdigit(int ch) {
    return __isdigit(ch);
}

//字母/数字
int isalnum(int ch) {
    return __isalnum(ch);
}

//空格
int isblank(int ch) {
    return __isblank(ch);
}

//控制符
int iscntrl(int ch) {
    return __iscntrl(ch);
}

//可输出字符
int isprint(int ch) {
    return __isprint(ch);
}

//可见字符
int isgraph(int ch) {
    return __isgraph(ch);
}

//符号
int ispunct(int ch) {
    return __ispunct(ch);
}

//16进制位
int isxdigit(int ch) {
    return __isxdigit(ch);
}

//8进制位
int isodigit(int ch) {
    return __isodigit(ch);
}

//大写字母->小写字母
int tolower(int ch) {
    return __tolower(ch);
}

//小写字母->大写字母
int toupper(int ch) {
    return __toupper(ch);
}