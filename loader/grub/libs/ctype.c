/**
 * @file ctype.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 标准库ctype实现
 * @version alpha-1.0.0
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <ctype.h>

int isspace(int ch) {
    return __isspace(ch);
}

int isupper(int ch) {
    return __isupper(ch);
}

int islower(int ch) {
    return __islower(ch);
}

int isalpha(int ch) {
    return __isalpha(ch);
}

int isdigit(int ch) {
    return __isdigit(ch);
}

int isalnum(int ch) {
    return __isalnum(ch);
}

int isblank(int ch) {
    return __isblank(ch);
}

int iscntrl(int ch) {
    return __iscntrl(ch);
}

int isprint(int ch) {
    return __isprint(ch);
}

int isgraph(int ch) {
    return __isgraph(ch);
}

int ispunct(int ch) {
    return __ispunct(ch);
}

int isxdigit(int ch) {
    return __isxdigit(ch);
}

int isodigit(int ch) {
    return __isodigit(ch);
}

int tolower(int ch) {
    return __tolower(ch);
}

int toupper(int ch) {
    return __toupper(ch);
}