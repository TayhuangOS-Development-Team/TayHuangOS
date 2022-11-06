/**
 * @file string.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief string
 * @version alpha-1.0.0
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */



#include <string.h>
#include <ctype.h>

int strcmp(const char *str1, const char *str2) {
    // 获取长度
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    // 长度不等比长度
    if (len1 != len2) {
        if (len1 < len2) {
            return -1;
        }
        else {
            return 1;
        }
    }
    //长度不等比差值
    int res = 0;
    while (*str1) {
        res = (*str1) - (*str2);
        if (res != 0) {
            break;
        }
        str1 ++;
        str2 ++;
    }

    if (res < 0) {
        return -1;
    }
    else if (res > 0) {
        return 1;
    }
    return 0;
}

int strlen(const char *str) {
    // 用扫描指令
    int cnt = 0x7FFFFFFF;
    __asm__ __volatile__ (
      "cld\n"
      "repnz\n"
      "scasb"  :
      "+c" (cnt) : "D" (str), "a" (0));
    return 0x7FFFFFFF - cnt - 1;
}

int wstrlen(const wchar_t *str) {
    int size = 0;
    while (*str != L'\0') {
        size ++;
        str ++;
    }
    return size;
}

char *strcpy(void *dst, const char *src) {
    // 挨个复制
    char *r = dst;
    while(*src != '\0') {
        *(r ++) = *(src ++);
    }
    *r = '\0';
    return dst;
}