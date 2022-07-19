/*
 * SPDX-License-Identifier: LGPL-2.1-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: theflysong
 *
 * printf.c
 *
 * 内核printf
 *
 */



#include <printf.h>

#include <tool/tostring.h>

#include <tayhuang/io.h>
#include <tayhuang/services.h>

#include <ctype.h>
#include <string.h>

//状态
PRIVATE int print_color = 0;
PRIVATE int tty = -1;

//获取/设置输出颜色
PUBLIC int get_print_color(void) {
    return print_color;
}

PUBLIC void set_print_color(int color) {
    print_color = color;
}

//设置tty
PUBLIC int get_tty(void) {
    return tty;
}

PUBLIC void set_tty(int _tty) {
    tty = _tty;
}

//TODO: 重写
#define TTY_PUTCHAR (0)
#define TTY_WRITE_STR (1)
#define TTY_CLEAR_SCREEN (2)
#define TTY_GETCHAR (3)
#define TTY_GETCHARS (4)
#define TTY_GETPOSX (5)
#define TTY_GETPOSY (6)
#define TTY_SETPOS (7)
#define TTY_SETACTIVE (8)
#define TTY_SETSCROLLLINE (9)
#define TTY_GETSCROLLLINE (10)

PUBLIC void set_active_tty(int _tty) {
    //qword command[] = {TTY_SETACTIVE, _tty};
    //send_msg(command, TTY_DRIVER_SERVICE, sizeof(command), 20);
}

PUBLIC void change_pos(int x, int y) {
    //qword command[] = {TTY_SETPOS, tty, x, y};
    //send_msg(command, TTY_DRIVER_SERVICE, sizeof(command), 20);
}

PUBLIC int get_pos_x(void) {
    int x = 0;
    //qword command[] = {TTY_GETPOSX, tty};
    //sendrecv(command, &x, TTY_DRIVER_SERVICE, sizeof(command), 20);
    return x;
}

PUBLIC int get_pos_y(void) {
    int y = 0;
    //qword command[] = {TTY_GETPOSY, tty};
    //sendrecv(command, &y, TTY_DRIVER_SERVICE, sizeof(command), 20);
    return y;
}

PUBLIC int get_scroll_line(void) {
    int scroll_line = 0;
    //qword command[] = {TTY_GETSCROLLLINE, tty};
    //sendrecv(command, &scroll_line, TTY_DRIVER_SERVICE, sizeof(command), 20);
    return scroll_line;
}

PUBLIC void set_scroll_line(int line) {
    //qword command[] = {TTY_SETSCROLLLINE, tty, line};
    //send_msg(command, TTY_DRIVER_SERVICE, sizeof(command), 20);
}

PUBLIC void clrscr(void) {
    //qword command[] = {TTY_CLEAR_SCREEN, tty};
    //send_msg(command, TTY_DRIVER_SERVICE, sizeof(command), 20);
}

#define NUM_MAX_CHARACTERS (64)

//缓冲区
PRIVATE qword command[NUM_MAX_CHARACTERS * 2 + 2];
PRIVATE int write_pos = 0;

//输出到屏幕上
PUBLIC void flush_to_screen(void) {
    command[0] = TTY_WRITE_STR;
    command[1] = tty;
    command[2] = write_pos;

    //send_msg (command, TTY_DRIVER_SERVICE, sizeof(command), 20);

    write_pos = 0;
}

//输出字符
PRIVATE void __putchar(char ch) {
    if (write_pos >= NUM_MAX_CHARACTERS) {
        flush_to_screen();
    }
    command[write_pos * 2 + 3] = ch;
    command[write_pos * 2 + 4] = print_color;
    write_pos ++;
}

//输出字符
PUBLIC void putchar(char ch) {
    __putchar(ch);
    flush_to_screen();
}

//输出字符串
PUBLIC void puts(const char *str) {
    while (*str)
        __putchar (*(str ++));
    flush_to_screen();
}

PUBLIC int vsprintf(char *buffer, const char *format, va_list args) {
    #define FLAG_SIGN 1
    #define FLAG_LEFT_ALIGN 2
    #define FLAG_FILL_ZERO 4
    #define FLAG_PREFIX 8
    #define FLAG_UPPER 16

    #define PRINT_TY_INT 0
    #define PRINT_TY_UNSIGNED 1
    #define PRINT_TY_FLOAT 2
    #define PRINT_TY_EXPONENT 3
    #define PRINT_TY_OCT 4
    #define PRINT_TY_HEX 5
    #define PRINT_TY_CHAR 6
    #define PRINT_TY_STRING 7

    #define QUAL_SHORT 0
    #define QUAL_NORMAL 1
    #define QUAL_LONG 2
    #define QUAL_LONGLONG 3

    char *original = buffer;

    while (*format) {
        //不是格式符
        if (*format != '%') {
            *buffer = *format;
            buffer ++;
            format ++;
        }
        else {
            format ++;

            if (*format == '%') {
                *buffer = '%';
                buffer ++;
                format ++;
                continue;
            }
            byte flag = 0;
            int width = 0;
            int precision = 0;
            byte print_type = PRINT_TY_INT;
            byte qualifier = QUAL_NORMAL;

            //设置标志位
            while (true) {
                bool _break = false;
                switch (*format) {
                case '+': flag |= FLAG_SIGN; format ++; break;
                case '-': flag |= FLAG_LEFT_ALIGN; format ++; break;
                case ' ': format ++; break;
                case '0': flag |= FLAG_FILL_ZERO; format ++; break;
                case '#': flag |= FLAG_PREFIX; format ++; break;
                default: _break = true; break;
                }
                if (_break) {
                    break;
                }
            }

            //是否左对齐
            if (*format == '*') {
                width = va_arg(args, int);
                if (width < 0) {
                    width = -width;
                    flag |= FLAG_LEFT_ALIGN;
                }
                format ++;
            }
            else {
                while (isdigit(*format)) {
                    width = width * 10 + *format - '0';
                    format ++;
                }
            }

            //精度
            if (*format == '.') {
                format ++;
                if (*format == '*') {
                    precision = va_arg(args, int);
                    format ++;
                }
                else {
                    while (isdigit(*format)) {
                        precision = precision * 10 + *format - '0';
                        format ++;
                    }
                }
            }

            //位长设置
            switch (*format) {
            case 'l': case 'L': qualifier = QUAL_LONG; format ++; break;
            case 'h': qualifier = QUAL_SHORT; format ++; break;
            }

            //位长设置(lld)
            switch (*format) {
            case 'l': case 'L': qualifier = QUAL_LONGLONG; format ++; break;
            }

            //输出类型
            switch (*format) {
            case 'd': print_type = PRINT_TY_INT; break;
            case 'u': print_type = PRINT_TY_UNSIGNED; break;
            case 'o': print_type = PRINT_TY_OCT; break;
            case 'X': flag |= FLAG_UPPER;
            case 'x': print_type = PRINT_TY_HEX; break;
            case 'f': print_type = PRINT_TY_FLOAT; break;
            case 'c': print_type = PRINT_TY_CHAR; break;
            case 's': print_type = PRINT_TY_STRING; break;
            case 'E': flag |= FLAG_UPPER;
            case 'e': print_type = PRINT_TY_EXPONENT; break;
            case 'P': flag |= FLAG_UPPER;
            case 'p': flag |= FLAG_FILL_ZERO; flag |= FLAG_PREFIX; width = 16; print_type = PRINT_TY_HEX; break;
            default: print_type = -1; break;
            }

            //没有输出类型
            if (print_type == -1) {
                return -1;
            }
            format ++;

            bool has_sign = false;

            int offset = 0;

            char _buffer[120] = {};

            //根据输出类型输出
            switch (print_type) {
            case PRINT_TY_INT: { //整形
                if (qualifier != QUAL_LONGLONG) { 
                    int val = va_arg(args, int);
                    if (val < 0) {
                        has_sign = true;
                        val = -val;
                    }
                    itoa(val, _buffer, 10);
                }
                else { //long long
                    long long val = va_arg(args, long long);
                    if (val < 0) {
                        has_sign = true;
                        val = -val;
                    }
                    lltoa(val, _buffer, 10);
                }
                break;
            }
            case PRINT_TY_UNSIGNED: { //无符号整形
                if (qualifier != QUAL_LONGLONG) {
                    unsigned int val = va_arg(args, unsigned int);
                    uitoa(val, _buffer, 10);
                }
                else { //long long
                    unsigned long long val = va_arg(args, unsigned long long);
                    ulltoa(val, _buffer, 10);
                }
                break;
            }
            case PRINT_TY_FLOAT: { //浮点数
                double val = va_arg(args, double);
                if (val < 0) {
                    has_sign = true;
                    val = -val;
                }
                dtoa(val, _buffer, precision);
                break;
            }
            case PRINT_TY_EXPONENT: { //浮点数(科学计数法)
                double val = va_arg(args, double);
                if (val < 0) {
                    has_sign = true;
                    val = -val;
                }
                dtoea(val, _buffer, flag & FLAG_UPPER);
                break;
            }
            case PRINT_TY_OCT: { //八进制
                unsigned int val = va_arg(args, unsigned int);
                uitoa(val, _buffer, 8);
            }
            case PRINT_TY_HEX: { //十六进制
                unsigned long long val = va_arg(args, unsigned long long);
                ulltoa(val, _buffer, 16);
                if (flag & FLAG_UPPER) {
                    for (int i = 0 ; i < strlen(_buffer) ; i ++) {
                        if (islower(_buffer[i])) {
                            _buffer[i] = toupper(_buffer[i]);
                        }
                    }
                }
                break;
            }
            case PRINT_TY_CHAR: { //字符
                char ch = (char)va_arg(args, unsigned int);
                *buffer = ch;
                buffer ++;
                offset ++;
                break;
            }
            case PRINT_TY_STRING: { //字符串
                char *str = va_arg(args, char*);
                strcpy(buffer, str);
                buffer += strlen(str);
                offset += strlen(str);
                break;
            }
            }

            if (flag & FLAG_SIGN) { //符号
                if (! has_sign) {
                    *buffer = '+';
                    buffer ++;
                    offset ++;
                }
            }

            if (has_sign) {
                *buffer = '-';
                buffer ++;
                offset ++;
            }


            if (flag & FLAG_PREFIX) { //前缀
                if (print_type == PRINT_TY_OCT) {
                    *buffer = '0';
                    buffer ++;
                }
                else if (print_type == PRINT_TY_HEX) {
                    *buffer = '0';
                    buffer ++;
                    *buffer = flag & FLAG_UPPER ? 'X' : 'x';
                    buffer ++;
                }
            }

            for (int i = strlen(_buffer) + offset ; i < width ; i ++) { //前导0
                *buffer = flag & FLAG_FILL_ZERO ? '0' : ' ';
                buffer ++;
            }

            strcpy(buffer, _buffer);
            buffer += strlen(_buffer);
        }
    }

    //结束符
    *buffer = '\0';

    //返回输出的字符的长度(不包括\0)
    return buffer - original - 1;

    #undef PRINT_TY_INT
    #undef PRINT_TY_UNSIGNED
    #undef PRINT_TY_FLOAT
    #undef PRINT_TY_EXPONENT
    #undef PRINT_TY_OCT
    #undef PRINT_TY_HEX
    #undef PRINT_TY_CHAR
    #undef PRINT_TY_STRING

    #undef FLAG_SIGN
    #undef FLAG_LEFT_ALIGN
    #undef FLAG_FILL_ZERO
    #undef FLAG_PREFIX
    #undef FLAG_UPPER
}

PUBLIC int printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    static char buffer[256];

    int ret = vsprintf(buffer, format, args);

    puts(buffer);

    va_end(args);
    return ret;
}

PUBLIC int sprintf(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);

    int ret = vsprintf(buffer, format, args);

    va_end(args);
    return ret;
}