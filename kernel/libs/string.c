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
 * string.c
 *
 * 标准库string实现
 *
 */



#include <string.h>
#include <ctype.h>

int strcmp(const char *str1, const char *str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    //长度不等
    if (len1 != len2) {
        if (len1 < len2) {
            return -1;
        }
        else {
            return 1;
        }
    }
    int res = 0;
    while (*str1 != '\0') {
        res = (*str1) - (*str2);
        //字符不等
        if (res != 0) {
            break;
        }
        str1 ++;
        str2 ++;
    }

    //前者小于后者
    if (res < 0) {
        return -1;
    }
    else if (res > 0) {
        return 1;
    }
    //二者相等
    return 0;
}

int strlen(const char *str) {
    int cnt = 0x7FFFFFFF;
    __asm__ __volatile__ (
      "cld\n"
      "repnz\n"
      "scasb"  :
      "+c" (cnt) : "D" (str), "a" (0));
    return 0x7FFFFFFF - cnt - 1;
}

char *strcpy(void *dst, const char *src) {
    char *r = dst;
    while(*src != '\0') {
        *(r ++) = *(src ++);
    }
    *r = '\0';
    return dst;
}