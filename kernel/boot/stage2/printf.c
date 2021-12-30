// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong Beanflame
 *
 * kernel/boot/stage2/print.c
 *
 * Display functions are implemented here
 */



#include "printf.h"
#include "drivers/drivers.h"
#include "drivers/devices.h"
#include "drivers/vedio/vedio_driver.h"
#include "intcall.h"
#include <ctype.h>
#include <stdarg.h>
#include <string.h>

#define PRINT_COLOR 0x0F

PRIVATE short print_x = 0;
PRIVATE short print_y = 0;

PRIVATE void set_cursor_pos(byte x, byte y) {
    intargs_t args;
    reg_collect_t in_regs;
    reg_collect_t out_regs;
    in_regs.eax = MKDWORD(0, MKWORD(2, 0));
    in_regs.ebx = MKDWORD(0, MKWORD(0, 0));
    in_regs.edx = MKDWORD(0, MKWORD(y, x));
    args.in_regs = &in_regs;
    args.out_regs = &out_regs;
    args.int_no = 0x10;
    intcall(&args);
}

PRIVATE APACK(vd, writebyte) write_pack;

PUBLIC void disp_char(char ch, byte color, byte x, byte y) {
    write_pack.pos_x = x;
    write_pack.pos_y = y;
    write_pack.ch = ch;
    write_pack.color = color;
    vedio_driver.pc_handle(&vedio_driver, VD_CMD_WRITE_BYTE, &write_pack);
}

PUBLIC void putchar(char ch) {
    disp_char(ch, PRINT_COLOR, print_x ++, print_y);
    if (print_x == 80) {
        print_x = 0;
        print_y ++;
    }
    set_cursor_pos(print_x, print_y);
}

PUBLIC void puts(const char *str) {
    while (*str != 0) {
        if (*str == '\n') {
            print_x = 0;
            print_y ++;
            str ++;
            set_cursor_pos(print_x, print_y);
        }
        else if (*str == '\t') {
            print_x += 4;
            set_cursor_pos(print_x, print_y);
        }
        else if (*str == '\v') {
            print_y ++;
            set_cursor_pos(print_x, print_y);
        }
        else if (*str == '\r') {
            print_x = 0;
            set_cursor_pos(print_x, print_y);
        }
        else if (*str == '\b') {
            print_x --;
            set_cursor_pos(print_x, print_y);
        }
        else if(*str == '\f') {
            clrscr();
            print_x = 0;
            print_y = 0;
            set_cursor_pos(print_x, print_y);
        }
        else {
            putchar(*(str ++));
        }
    }
}

PUBLIC void change_pos(word x, word y) {
    print_x = x;
    print_y = y;
}

PUBLIC word get_pos_x(void) {
    return print_x;
}

PUBLIC word get_pos_y(void) {
    return print_y;
}

PUBLIC void clrscr(void) {
    vedio_driver.pc_handle(&vedio_driver, VD_CMD_CLRSCR, NULL);
}

PRIVATE int _vsprintf(char* buffer, const char* format, va_list args) {
    bool flag1, flag2;
    int flag3;
    char* store = buffer;
    flag1 = flag2 = false;
    flag3 = 0;
    while (*format != 0) {
        if (*format == '%') {
            format ++;
            if (*format == '%') {
                *(buffer ++) = *format;
                format ++;
                continue;
            }
            else if (*format == '#') {
                flag1 = true;
                format ++;
            }
            else if (*format == '+') {
                flag2 = true;
                format ++;
            }
            if (isdigit(*format)) {
                flag3 = 0;
                while (isdigit(*format)) {
                    flag3 = flag3 * 10 + (*format) - '0';
                    format ++;
                }
            }
            if (*format == 'd') {
                int num = va_arg(args, int);
                if (flag2) {
                    if (num > 0) {
                        *(buffer ++) = '+';
                    }
                }
                char _buff[100];
                char* buff = _buff;
                itoa(buff, num, 10);
                int len = strlen(buff);
                if (flag3 > len) {
                    for (int i = 0 ; i < flag3 - len ; i ++) {
                        *(buffer ++) = '0';
                    }
                }
                while (*buff != 0) {
                    *(buffer ++) = *(buff ++);
                }
            }
            else if (*format == 'u') {
                unsigned int num = va_arg(args, unsigned int);
                if (flag2) {
                    if (num > 0) {
                        *(buffer ++) = '+';
                    }
                }
                char _buff[100];
                char* buff = _buff;
                uitoa(buff, num, 10);
                int len = strlen(buff);
                if (flag3 > len) {
                    for (int i = 0 ; i < flag3 - len ; i ++) {
                        *(buffer ++) = '0';
                    }
                }
                while (*buff != 0) {
                    *(buffer ++) = *(buff ++);
                }
            }
            else if (*format == 'c') {
                char ch = va_arg(args, char);
                *(buffer ++) = ch;
            }
            else if (*format == 's') {
                const char* str;
                str = va_arg(args, const char*);
                while (*str != 0) {
                    *(buffer ++) = *(str ++);
                }
            }
            else if (*format == 'o') {
                if (flag1) {
                    *(buffer ++) = '0';
                }
                unsigned int num = va_arg(args, unsigned int);
                char _buff[100];
                char* buff = _buff;
                uitoa(buff, num, 8);
                int len = strlen(buff);
                if (flag3 > len) {
                    for (int i = 0 ; i < flag3 - len ; i ++) {
                        *(buffer ++) = '0';
                    }
                }
                while (*buff != 0) {
                    *(buffer ++) = *(buff ++);
                }
            }
            else if (*format == 'x') {
                if (flag1) {
                    *(buffer ++) = '0';
                    *(buffer ++) = 'x';
                }
                unsigned int num = va_arg(args, unsigned int);
                char _buff[100];
                char* buff = _buff;
                uitoa(buff, num, 16);
                int len = strlen(buff);
                if (flag3 > len) {
                    for (int i = 0 ; i < flag3 - len ; i ++) {
                        *(buffer ++) = '0';
                    }
                }
                while (*buff != 0) {
                    *(buffer ++) = *(buff ++);
                }
            }
            else if (*format == 'X') {
                if (flag1) {
                    *(buffer ++) = '0';
                    *(buffer ++) = 'X';
                }
                unsigned int num = va_arg(args, unsigned int);
                char _buff[100];
                char* buff = _buff;
                uitoa_upper(buff, num, 16);
                int len = strlen(buff);
                if (flag3 > len) {
                    for (int i = 0 ; i < flag3 - len ; i ++) {
                        *(buffer ++) = '0';
                    }
                }
                while (*buff != 0) {
                    *(buffer ++) = *(buff ++);
                }
            }
            format ++;
        }
        else {
            *(buffer ++) = *(format ++);
        }
        flag1 = flag2 = false;
        flag3 = 0;
    }
    *buffer = '\0';
    return buffer - store;
}

//输出内容到vsprintf中
PUBLIC int vsprintf(char* buffer, const char* format, ...) {
    va_list args;
    va_start(args, format);

    int res = _vsprintf(buffer, format, args);

    va_end(args);

    return res;
}


PUBLIC int printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    char buffer[128];
    int res = _vsprintf(buffer, format, args);
    puts(buffer);

    va_end(args);

    return res;
}