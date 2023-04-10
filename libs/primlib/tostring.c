/**
 * @file tostring.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief tostring - 实现
 * @version alpha-1.0.0
 * @date 2023-04-09
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <primlib/tostring.h>

// 数字表
static const char *digits = "0123456789abcdefghijklmnopqrstuvwxyz";

char *uitoa(unsigned int val, char *buffer, int base) {
    char *ret = buffer;
    // 特殊情况
    if (val == 0) {
        *buffer = '0';
        buffer ++;
        *buffer = '\0';
        return ret;
    }

    char _buffer[12];
    int cnt = 0;

    // val仍大于0
    while (val > 0) {
        // 保存该位数字
        _buffer[cnt] = digits[val % base];

        cnt ++;

        // 右移一个base进制位
        val /= base;
    }

    // 恢复原顺序
    for (int i = cnt - 1; i >=  0 ; i --) {
        *buffer = _buffer[i];
        buffer ++;
    }

    return ret;
}

char *itoa(int val, char *buffer, int base) {
    // 保存起始位置
    char *ret = buffer;

    // 特殊情况判断
    if (val == 0) {
        *buffer = '0';
        buffer ++;
        *buffer = '\0';
        return ret;
    }

    if (val < 0) {
        *buffer = '-';
        buffer ++;
        val = -val;
    }
    
    // 正整数
    uitoa(val, buffer, base);

    return ret;
}
