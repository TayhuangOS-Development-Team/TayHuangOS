// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights resulterved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage2/std/string.c
 *
 * 标准库string.h的源文件
 */



#include <string.h>
#include <ctype.h>

int strcmp(const char* str1, const char* str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    if (len1 != len2) { //长度不等
        if (len1 < len2)
            return -1;
        else 
            return 1;
    }
    int result = 0;
    while (*str1) {
        result = (*str1) - (*str2);
        if (result != 0) break; //某个字符不等
        str1 ++;
        str2 ++;
    }

    if (result < 0)
        return -1;
    else if (result > 0)
        return 1;
    return 0;
}

int strlen(const char* str) {
    int len = 0;
    while (*(str ++)) len ++; //不是'\0'则长度+1
    return len;
}

char* strcpy(char* dst, const char* src) {
    char* r = dst;
    do {
        *(r ++) = *(src ++);
    }
    while(*src != '\0'); //是'\0'则结束复制
    return dst;
}

int atoi(const char* str) {
    int resultult = 0;
    char sign = 1;

    if (str == 0) {
        return 0;
    }

    while (isspace(*(str ++))) {}
    str --;

    if ((*str) == '-') sign = -1;
    if ((*str) == '-' || (*str) == '+') str ++;

    while (isdigit(*str)) {
        resultult = resultult * 10 + (*str) - '0';
        str ++;
    }

    return sign * resultult;
}

int atoi_8(const char* str) {
    int resultult = 0;
    char sign = 1;

    if (str == 0) {
        return 0;
    }

    while (isspace(*(str ++))) {}
    str --;

    if ((*str) == '-') sign = -1;
    if ((*str) == '-' || (*str) == '+') str ++;

    while (isodigit(*str)) {
        resultult = resultult * 8 + (*str) - '0';
        str ++;
    }

    return sign * resultult;
}

int atoi_16(const char* str) {
    int resultult = 0;
    char sign = 1;

    if (str == 0) {
        return 0;
    }

    while (isspace(*(str ++))) {}
    str --;

    if ((*str) == '-') sign = -1;
    if ((*str) == '-' || (*str) == '+') str ++;

    while (isxdigit(*str)) {
        resultult = resultult * 16 + tolower(*str) - (isdigit(*str) ? '0' : 'a');
        str ++;
    }

    return sign * resultult;
}

unsigned int atoui(const char* str) {
    unsigned int resultult = 0;

    if (str == 0) {
        return 0;
    }

    while (isspace(*(str ++))) {}
    str --;

    if ((*str) == '+') str ++;

    while (isdigit(*str)) {
        resultult = resultult * 10 + (*str) - '0';
        str ++;
    }

    return resultult;
}

unsigned int atoui_8(const char* str) {
    unsigned int resultult = 0;

    if (str == 0) {
        return 0;
    }

    while (isspace(*(str ++))) {}
    str --;

    if ((*str) == '+') str ++;

    while (isodigit(*str)) {
        resultult = resultult * 8 + (*str) - '0';
        str ++;
    }

    return resultult;
}

unsigned int atoui_16(const char* str) {
    unsigned int resultult = 0;

    if (str == 0) {
        return 0;
    }

    while (isspace(*(str ++))) {}
    str --;
    if ((*str) == '+') str ++;

    while (isxdigit(*str)) {
        resultult = resultult * 16 + tolower(*str) - (isdigit(*str) ? '0' : 'a');
        str ++;
    }

    return resultult;
}

static const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";

char* itoa(char* buffer, int value, int radix) {
    char* store = buffer;
    if (radix < 2 || radix >= 36) {
        *buffer = 0;
        return store;
    }
    if (value == 0) {
        *(buffer ++) = '0';
        *buffer = 0;
        return store;
    }
    else if (value < 0) {
        *(buffer ++) = '-';
        value = -value;
    }

    char buf[100] = {};

    int cnt = 0;

    while (value > 0) {
        buf[cnt ++] = digits[value % radix];
        value /= radix;
    }

    for (cnt -= 1 ; cnt >= 0 ; cnt --) {
        *(buffer ++) = buf[cnt];
    }
    *buffer = 0;

    return store;
}

char* uitoa(char* buffer, unsigned int value, int radix) {
    char* store = buffer;
    if (radix < 2 || radix >= 36) {
        *buffer = 0;
        return store;
    }
    if (value == 0) {
        *(buffer ++) = '0';
        *buffer = 0;
        return store;
    }

    char buf[100] = {};

    int cnt = 0;

    while (value > 0) {
        buf[cnt ++] = digits[value % radix];
        value /= radix;
    }

    for (cnt -= 1 ; cnt >= 0 ; cnt --) {
        *(buffer ++) = buf[cnt];
    }
    *buffer = 0;
    return store;
}

static const char upper_digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

char* itoa_upper(char* buffer, int value, int radix) {
    char* store = buffer;
    if (radix < 2 || radix >= 36) {
        *buffer = 0;
        return store;
    }
    if (value == 0) {
        *(buffer ++) = '0';
        *buffer = 0;
        return store;
    }
    else if (value < 0) {
        *(buffer ++) = '-';
        value = -value;
    }

    char buf[100] = {};

    int cnt = 0;

    while (value > 0) {
        buf[cnt ++] = upper_digits[value % radix];
        value /= radix;
    }

    for (cnt -= 1 ; cnt >= 0 ; cnt --) {
        *(buffer ++) = buf[cnt];
    }
    *buffer = 0;

    return store;
}

char* uitoa_upper(char* buffer, unsigned int value, int radix) {
    char* store = buffer;
    if (radix < 2 || radix >= 36) {
        *buffer = 0;
        return store;
    }
    if (value == 0) {
        *(buffer ++) = '0';
        *buffer = 0;
        return store;
    }
    else if (value < 0) {
        *(buffer ++) = '-';
        value = -value;
    }

    char buf[100] = {};

    int cnt = 0;

    while (value > 0) {
        buf[cnt ++] = upper_digits[value % radix];
        value /= radix;
    }

    for (cnt -= 1 ; cnt >= 0 ; cnt --) {
        *(buffer ++) = buf[cnt];
    }
    *buffer = 0;

    return store;
}

char* ftoa(char* buffer, float value) {
    char* store = buffer;
    if (value == 0) {
        *(buffer ++) = '0';
        *buffer = 0;
        return store;
    }
    else if (value < 0) {
        *(buffer ++) = '-';
        value = -value;
    }

    char buf[100] = {};

    int cnt = 0;

    if (((int)value) == 0) {
        *(buffer ++) = '0';
    }

    while (value > 1) {
        buf[cnt ++] = ((int)value) % 10 + '0';
        value /= 10;
    }
    for (cnt -= 1 ; cnt >= 0 ; cnt --) {
        *(buffer ++) = buf[cnt];
    }

    *(buffer ++) = '.';

    while (value > 0) {
        value *= 10;
        *(buffer ++) = (int)value + '0';
        value -= (int)value;
    }

    *buffer = 0;
    return store;
}