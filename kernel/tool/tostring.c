/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 * 
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 * 
 * --------------------------------------------------------------------------------
 * 
 * 作者: Flysong
 * 
 * tostring.c
 * 
 * xx转字符串
 * 
 */



#include <tool/tostring.h>
#include <tayhuang/defs.h>

PRIVATE const char *digits = "0123456789abcedfghijklmnopqrstuvwxyz";

char *itoa(int val, char *buffer, int base) {
    char *save = buffer;
    if (val == 0) {
        *buffer = '0';
        buffer ++;
        *buffer = '\0';
        return save;
    }
    if (val < 0) {
        *buffer = '-';
        buffer ++;
        val = -val;
    }
    char _buffer[12];
    int cnt = 0;
    while (val > 0) {
        _buffer[cnt] = digits[val % base];
        cnt ++;
        val /= base;
    }
    for (int i = cnt - 1; i >=  0 ; i --) {
        *buffer = _buffer[i];
        buffer ++;
    }
    return save;
}

char *uitoa(unsigned int val, char *buffer, int base) {
    char *save = buffer;
    if (val == 0) {
        *buffer = '0';
        buffer ++;
        *buffer = '\0';
        return save;
    }
    char _buffer[12];
    int cnt = 0;
    while (val > 0) {
        _buffer[cnt] = digits[val % base];
        cnt ++;
        val /= base;
    }
    for (int i = cnt - 1; i >=  0 ; i --) {
        *buffer = _buffer[i];
        buffer ++;
    }
    return save;
}

char *lltoa(long long val, char *buffer, int base) {
    char *save = buffer;
    if (val == 0) {
        *buffer = '0';
        buffer ++;
        *buffer = '\0';
        return save;
    }
    if (val < 0) {
        *buffer = '-';
        buffer ++;
        val = -val;
    }
    char _buffer[23];
    int cnt = 0;
    while (val > 0) {
        _buffer[cnt] = digits[val % base];
        cnt ++;
        val /= base;
    }
    for (int i = cnt - 1; i >=  0 ; i --) {
        *buffer = _buffer[i];
        buffer ++;
    }
    return save;
}

char *ulltoa(unsigned long long val, char *buffer, int base) {
    char *save = buffer;
    if (val == 0) {
        *buffer = '0';
        buffer ++;
        *buffer = '\0';
        return save;
    }
    char _buffer[23];
    int cnt = 0;
    while (val > 0) {
        _buffer[cnt] = digits[val % base];
        cnt ++;
        val /= base;
    }
    for (int i = cnt - 1; i >=  0 ; i --) {
        *buffer = _buffer[i];
        buffer ++;
    }
    return save;
}

char *ftoa(float val, char *buffer, int round) {
    return buffer;
}

char *dtoa(double val, char *buffer, int round) {
    return buffer;
}

char *ftoea(float val, char *buffer, unsigned char upper_e) {
    // int v = *(int*)&val;
    // int sign = v >> 31;
    // int exponent = (v >> 23 & 0xFF) - 128;
    // int fraction = v & 0x7FFFFF;
    return buffer;
}

char *dtoea(double val, char *buffer, unsigned char upper_e) {
    // long long v = *(long long*)&val;
    // int sign = v >> 63;
    // int exponent = (v >> 51 & 0x7FF) - 128;
    // long long fraction = v & 0x7FFFFFFFFFFFF;
    return buffer;
}