/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * printf.h
 *
 * 打印函数
 *
 */



#include "printf.h"
#include "video.h"
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

PRIVATE byte print_color = 0x0F;
PUBLIC struct print_info_t print_info;

PUBLIC byte get_print_color(void) {
    return print_color;
}

PUBLIC void set_print_color(byte color) {
    print_color = color;
}

PRIVATE void set_cursor_pos(byte x, byte y) {
    word position = y * DPINFO.screen_width + x;
    outb(0x3D4, 0x0F);
	outb(0x3D5, LOWBYTE(position));
	outb(0x3D4, 0x0E);
	outb(0x3D5, HIGHBYTE(position));
}

PUBLIC void scroll_screen(int num) {
    if (num > 0) { //上滚
        //TODO: Scroll up
    } 
    else { //下滚
        //TODO: Scroll down
    }
}

PRIVATE short print_x = 0;
PRIVATE short print_y = 0;

PUBLIC void putchar(char ch) {
    if (ch == '\r' || ch == '\n') { //制表符
        print_x = 0;
        print_y ++;
    }
    else if (ch == '\t') {
        print_x += 4;
    }
    else if (ch == '\v') {
        print_y ++;
    }
    else if (ch == '\b') {
        if (print_x > 0) {
            draw_character(-- print_x, print_y, ' ', print_color);
        }
    }
    else if(ch == '\f') {
        clrscr();
        print_x = 0;
        print_y = 0;
    }
    else { //普通字符
        draw_character(print_x ++, print_y, ch, print_color);
    }
    if (print_x == DPINFO.screen_width) { //自动换行
        print_x = 0;
        print_y ++;
    }
    if (print_y - print_info.scroll_line > 0) { //滚动屏幕
        scroll_screen(print_y - print_info.scroll_line);
        print_y = print_info.scroll_line;
    }
    set_cursor_pos(print_x, print_y);
}

PUBLIC void puts(const char *str) {
    while (*str != '\0') {
        char ch = *str;
        putchar(ch);
        str ++;
    }
}

PUBLIC void change_pos(word x, word y) {
    print_x = x;
    print_y = y;
    set_cursor_pos(print_x, print_y);
}

PUBLIC word get_pos_x(void) {
    return print_x;
}

PUBLIC word get_pos_y(void) {
    return print_y;
}

PUBLIC void clrscr(void) {
    //通过画空格实现清屏
    for (int i = 0 ; i < DPINFO.screen_width ; i ++) {
        for (int j = 0 ; j < DPINFO.screen_height ; j ++) {
            draw_character(i, j, ' ', print_color);
        }
    }
    print_x = 0;
    print_y = 0;
}

//基础vsprintf实现
PRIVATE int _vsprintf(char *buffer, const char *format, va_list args) {
    bool flag1, flag2;
    int flag3;
    char *store = buffer;
    flag1 = flag2 = false;
    flag3 = 0;
    while (*format != 0) {
        if (*format == '%') {
            format ++;
            if (*format == '%') { //输出%
                *(buffer ++) = *format;
                format ++;
                continue;
            }
            else if (*format == '#') { //16进制加0x
                flag1 = true;
                format ++;
            }
            else if (*format == '+') { //输出+
                flag2 = true;
                format ++;
            }
            if (isdigit(*format)) { //设置长度
                flag3 = 0;
                while (isdigit(*format)) {
                    flag3 = flag3 * 10 + (*format) - '0';
                    format ++;
                }
            }
            if (*format == 'd') { //整数
                int num = va_arg(args, int);
                if (flag2) {
                    if (num > 0) {
                        *(buffer ++) = '+';
                    }
                }
                char _buff[100];
                char *buff = _buff;
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
            else if (*format == 'u') { //无符号整数
                unsigned int num = va_arg(args, unsigned int);
                if (flag2) {
                    if (num > 0) {
                        *(buffer ++) = '+';
                    }
                }
                char _buff[100];
                char *buff = _buff;
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
            else if (*format == 'c') { //字符
                char ch = va_arg(args, char);
                *(buffer ++) = ch;
            }
            else if (*format == 's') { //字符串
                const char *str;
                str = va_arg(args, const char*);
                while (*str != 0) {
                    *(buffer ++) = *(str ++);
                }
            }
            else if (*format == 'o') { //八进制数
                if (flag1) {
                    *(buffer ++) = '0';
                }
                unsigned int num = va_arg(args, unsigned int);
                char _buff[100];
                char *buff = _buff;
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
            else if (*format == 'x') { //16进制数
                if (flag1) {
                    *(buffer ++) = '0';
                    *(buffer ++) = 'x';
                }
                unsigned int num = va_arg(args, unsigned int);
                char _buff[100];
                char *buff = _buff;
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
            else if (*format == 'X') { //大写的十六进制数
                if (flag1) {
                    *(buffer ++) = '0';
                    *(buffer ++) = 'X';
                }
                unsigned int num = va_arg(args, unsigned int);
                char _buff[100];
                char *buff = _buff;
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

//输出内容到buffer中
PUBLIC int vsprintf(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);

    int res = _vsprintf(buffer, format, args);

    va_end(args);

    return res;
}

//输出内容到屏幕中
PUBLIC int printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    char buffer[256];
    int res = _vsprintf(buffer, format, args);
    puts(buffer);

    va_end(args);

    return res;
}