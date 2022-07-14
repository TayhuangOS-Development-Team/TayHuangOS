/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: theflysong
 *
 * tostring.c
 *
 * xx转字符串
 *
 */



#include <tool/tostring.h>

static const char *digits = "0123456789abcdefghijklmnopqrstuvwxyz";

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