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
 * ctype.h
 *
 * 标准库ctype实现
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