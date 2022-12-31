/**
 * @file ctype.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief ctype.h
 * @version alpha-1.0.0
 * @date 2022-12-31
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#pragma once

// 是否为空格字符
static inline int isspace(int ch) {
    return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch == '\v' || ch == '\f';
}

// 是否为大写字母
static inline int isupper(int ch) {
    return ch >= 'A' && ch <= 'Z';
}

// 是否为小写字母
static inline int islower(int ch) {
    return ch >= 'a' && ch <= 'z';
}

// 是否为字母
static inline int isalpha(int ch) {
    return isupper(ch) || islower(ch);
}

// 是否为数字
static inline int isdigit(int ch) {
    return ch >= '0' && ch <= '9';
}

// 是否为数字或字母
static inline int isalnum(int ch) {
    return isalpha(ch) || isdigit(ch);
}

// 是否为空白字符
static inline int isblank(int ch) {
    return ch == ' ' || ch == '\t';
}

// 是否为控制字符
static inline int iscntrl(int ch) {
    return ((ch >= 0x00 && ch <= 0x1f) || ch == 0x7f);
}

// 是否为可打印字符
static inline int isprint(int ch) {
    return (! iscntrl(ch));
}

// 是否为可打印字符(非空格)
static inline int isgraph(int ch) {
    return isprint(ch) && (! isspace(ch));
}

// 是否为标点符号
static inline int ispunct(int ch) {
    return isgraph(ch) && (! isalnum(ch));
}

// 是否为十六进制字符
static inline int isxdigit(int ch) {
    return isalnum(ch) || ((ch >= 'a' && ch <= 'F') || (ch >= 'A' && ch <= 'F'));
}

// 是否为八进制字符
static inline int isodigit(int ch) {
    return ch >= '0' && ch <= '7';
}

// 大写转小写
static inline int tolower(int ch) {
    return isupper(ch) ? (ch - 'A' + 'a') : (ch);
}

// 小写转大写
static inline int toupper(int ch) {
    return islower(ch) ? (ch - 'a' + 'A') : (ch);
}