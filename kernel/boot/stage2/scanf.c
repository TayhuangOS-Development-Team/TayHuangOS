// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/scanf.c
 *
 * Input functions are implemented here
 */



#include "scanf.h"
#include "./drivers/drivers.h"
#include "./drivers/keyboard/keyboard_driver.h"
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include "./printf.h"

PUBLIC int getchar(void) {
    word ch;
    keyboard_driver.pc_handle(&keyboard_driver, KB_CMD_READ_KEY, &ch);
    putchar(ch&0xFF);
    if ((ch&0xFF) == '\r') putchar('\n');
    return ch & 0xFF;
}

#define SCAN_SZ_BYTE (0)
#define SCAN_SZ_WORD (1)
#define SCAN_SZ_DWORD (2)

PRIVATE void _assign(void* ptr, int sz, int value) {
    if (sz == SCAN_SZ_BYTE) {
        *(char*)ptr = value;
    }
    else if (sz == SCAN_SZ_WORD) {
        *(short*)ptr = value;
    }
    else if (sz == SCAN_SZ_DWORD) {
        *(int*)ptr = value;
    }
}

PRIVATE void _assignu(void* ptr, int sz, unsigned int value) {
    if (sz == SCAN_SZ_BYTE) {
        *(byte*)ptr = value;
    }
    else if (sz == SCAN_SZ_WORD) {
        *(word*)ptr = value;
    }
    else if (sz == SCAN_SZ_DWORD) {
        *(dword*)ptr = value;
    }
}

PRIVATE int _vscanf(rdkey_t rd_func, const char* format, va_list args) {
    int sum = 0;
    while (*format != 0) {
        if (*format == '%') {
            sum ++;
            int width = -1;
            int sz = SCAN_SZ_DWORD;
            bool flag1 = false;
            format ++;
            if (*format == '*') {
                flag1 = true;
                format ++;
            }
            if (isdigit(*format)) {
                width = 0;
                while (isdigit(*format)) {
                    width = width * 10 + (*format) - '0';
                    format ++;
                }
            }
            if (*format == 'h') {
                sz = SCAN_SZ_WORD;
                format ++;
            }
            if (*format == 'h') {
                if (sz == SCAN_SZ_WORD) {
                    sz = SCAN_SZ_BYTE;
                }
                format ++;
            }
            else if (* format == 'l') {
                sz = SCAN_SZ_DWORD;
                format ++;
            }
            else if (*format == 'j') {
                sz = SCAN_SZ_DWORD;
            }
            else if (*format == 'z') {
                sz = SCAN_SZ_DWORD;
            }
            else if (*format == 't') {
                sz = SCAN_SZ_DWORD;
            }
            if (*format == 'c') {
                if (width == -1) {
                    width = 1;
                }
                if (flag1) {
                    while (width --) rd_func();
                    continue;
                }
                char* ch = va_arg(args, char*);
                while (width --) {
                    *(ch ++) = rd_func();
                }
            }
            else if (*format == 'd') {
                char buffer[100];
                char ch = rd_func();
                int cnt = 0;
                while ((width --) && isdigit(ch)) {
                    buffer[cnt ++] = ch;
                    ch = rd_func();
                }
                if (flag1) continue;
                buffer[cnt] = 0;
                int value = atoi(buffer);
                _assign(va_arg(args, void*), sz, value);
            }
            else if (*format == 'u') {
                char buffer[100];
                char ch = rd_func();
                int cnt = 0;
                while ((width --) && isdigit(ch)) {
                    buffer[cnt ++] = ch;
                    ch = rd_func();
                }
                if (flag1) continue;
                buffer[cnt] = 0;
                unsigned int value = atoui(buffer);
                _assignu(va_arg(args, void*), sz, value);
            }
            else if (*format == 'o') {
                char buffer[100];
                char ch = rd_func();
                int cnt = 0;
                while ((width --) && isodigit(ch)) {
                    buffer[cnt ++] = ch;
                    ch = rd_func();
                }
                if (flag1) continue;
                buffer[cnt] = 0;
                int value = atoi_8(buffer);
                _assign(va_arg(args, void*), sz, value);
            }
            else if (*format == 'x' || *format == 'X') {
                char buffer[100];
                char ch = rd_func();
                int cnt = 0;
                while ((width --) && isxdigit(ch)) {
                    buffer[cnt ++] = ch;
                    ch = rd_func();
                }
                if (flag1) continue;
                buffer[cnt] = 0;
                int value = atoi_16(buffer);
                _assign(va_arg(args, void*), sz, value);
            }
            else if (*format == 's') {
                char* str = va_arg(args, char*);
                char ch = rd_func();
                if (flag1) {
                    while ((width --) && (!isspace(ch))) ch = rd_func();
                    continue;
                }
                while ((width --) && (!isspace(ch))) {
                    *(str ++) = ch;
                    ch = rd_func();
                }
            }
            format ++;
        }
        else {
            char ch = rd_func();
            if (ch == *format) {
                format ++;
            }
            else {
                break;
            }
        }
    }
    return sum;
}

PUBLIC int scanf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    int res = _vscanf(getchar, format, args);

    va_end(args);

    return res;
}

PUBLIC int vscanf(rdkey_t rd_func, const char* format, ...) {
    va_list args;
    va_start(args, format);

    int res = _vscanf(rd_func, format, args);

    va_end(args);

    return res;
}