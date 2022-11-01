/**
 * @file tostring.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief tostring
 * @version alpha-1.0.0
 * @date 2022-10-31
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */



#include <tool/tostring.h>

static const char *digits = "0123456789abcdefghijklmnopqrstuvwxyz";

char *itoa(int val, char *buffer, int base) {
    // 保存缓存
    char *save = buffer;
    // 特例
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
    // 每次除以基数取余数
    while (val > 0) {
        _buffer[cnt] = digits[val % base];
        cnt ++;
        val /= base;
    }
    // 反向
    for (int i = cnt - 1; i >=  0 ; i --) {
        *buffer = _buffer[i];
        buffer ++;
    }
    return save;
}

char *uitoa(unsigned int val, char *buffer, int base) {
    // 保存缓存
    char *save = buffer;
    // 特例
    if (val == 0) {
        *buffer = '0';
        buffer ++;
        *buffer = '\0';
        return save;
    }
    char _buffer[12];
    int cnt = 0;
    // 除以基数取余数
    while (val > 0) {
        _buffer[cnt] = digits[val % base];
        cnt ++;
        val /= base;
    }
    // 反向
    for (int i = cnt - 1; i >=  0 ; i --) {
        *buffer = _buffer[i];
        buffer ++;
    }
    return save;
}