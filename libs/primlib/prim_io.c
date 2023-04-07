/**
 * @file prim_io.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief primitive io - 实现
 * @version alpha-1.0.0
 * @date 2023-04-08
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */

#include <primlib/prim_io.h>

static int __llprintf(char *buffer, const char *fmt, va_list args) {
    return 0;
}

int vpprintf(prim_output_func func, const char *fmt, va_list args) {
    char buffer[512];

    int ret = __llprintf(buffer, fmt, args);
    func(buffer);

    return ret;
}

int vsprintf(char *buffer, const char *fmt, va_list args) {
    return __llprintf(buffer, fmt, args);
}

int pprintf(prim_output_func func, const char *fmt, ...) {
    va_list lst;
    va_start(lst, fmt);

    int ret = vpprintf(func, fmt, lst);

    va_end(lst);
    return ret;
}

int sprintf(char *buffer, const char *fmt, ...)  {
    va_list lst;
    va_start(lst, fmt);

    int ret = vsprintf(buffer, fmt, lst);

    va_end(lst);
    return ret;
}

int vpscanf(prim_getchar_func func, const char *fmt, va_list args) {
    return 0;
}

int pscanf(prim_getchar_func func, const char *fmt, ...) {
    va_list lst;
    va_start(lst, fmt);

    int ret = vpscanf(func, fmt, lst);

    va_end(lst);
    return ret;
}