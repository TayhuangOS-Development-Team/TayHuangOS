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
 * string.c
 *
 * Standard lib string.h source here
 *
 */



#include <string.h>
#include <ctype.h>

int strcmp(const char *str1, const char *str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    if (len1 != len2) {
        if (len1 < len2)
            return -1;
        else
            return 1;
    }
    int res = 0;
    while (*str1) {
        res = (*str1) - (*str2);
        if (res != 0) break;
        str1 ++;
        str2 ++;
    }

    if (res < 0)
        return -1;
    else if (res > 0)
        return 1;
    return 0;
}

int strlen(const char *str) {
    int cnt;
    asm volatile(
      "cld\n"
      "repnz\n"
      "scasb"  :
      "=c" (cnt) : "D" (str), "a" (0));
    return -2 - cnt;
}
char *strcpy(void *dst, const char *src) {
    char *r = dst;
    while(*src != '\0') {
        *(r ++) = *(src ++);
    }
    *r = '\0';
    return dst;
}