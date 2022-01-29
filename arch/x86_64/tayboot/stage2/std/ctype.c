// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/lib/std/ctype.h
 *
 * Standard lib ctype.h source here
 */

#include <ctype.h>

int isspace(int ch) {
    return _macro_isspace(ch);
}

int isupper(int ch) {
    return _macro_isupper(ch);
}

int islower(int ch) {
    return _macro_islower(ch);
}

int isalpha(int ch) {
    return _macro_isalpha(ch);
}

int isdigit(int ch) {
    return _macro_isdigit(ch);
}

int isalnum(int ch) {
    return _macro_isalnum(ch);
}

int isblank(int ch) {
    return _macro_isblank(ch);
}

int iscntrl(int ch) {
    return _macro_iscntrl(ch);
}

int isprint(int ch) {
    return _macro_isprint(ch);
}

int isgraph(int ch) {
    return _macro_isgraph(ch);
}

int ispunct(int ch) {
    return _macro_ispunct(ch);
}

int isxdigit(int ch) {
    return _macro_isxdigit(ch);
}

int isodigit(int ch) {
    return _macro_isodigit(ch);
}

int tolower(int ch) {
    return _macro_tolower(ch);
}

int toupper(int ch) {
    return _macro_toupper(ch);
}