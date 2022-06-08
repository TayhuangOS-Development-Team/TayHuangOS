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
 * printf.c
 *
 * 内核printf
 *
 */



#include "printf.h"

#include <tool/tostring.h>

#include <tayhuang/io.h>
#include <tayhuang/services.h>

#include <stdarg.h>
#include <ctype.h>
#include <string.h>

#include <ipc/ipc.h>

#define OUTPUT_BUFFER_SIZE (2000 + 80 * 250)
static int pos_x;
static int pos_y;
static int print_color = 0;
static int scroll_line = 0;
static bool do_cursor_move = true;

static void set_cursor_pos(int pos_x, int pos_y) {
    //TODO: Set Cursor Position
}

int get_print_color(void) {
    return print_color;
}

void set_print_color(int color) {
    print_color = color;
}

void change_pos(int x, int y) {
    pos_x = pos_x;
    pos_y = pos_y;
    if (do_cursor_move)
        set_cursor_pos(pos_x, pos_y);
}

int get_pos_x(void) {
    return pos_x;
}

int get_pos_y(void) {
    return pos_y;
}

int get_scroll_line(void) {
    return scroll_line;
}

void set_scroll_line(int line) {
    scroll_line = line;
}

void clrscr(void) {
    //TODO: Clear Screen
}

void scroll_screen(int lines) {
    //TODO: Scroll the screen
}

static void __update_pos(void) {
    if (pos_x >= 80) {
        pos_y += pos_x / 80;
        pos_x %= 80;
    }
}

enum {
    MODE_ANY = 0,
    MODE_TEXT = 1,
    MODE_GRAPHIC = 2
};

#define MKCMD(mode, code) ((((qword)(code & 0xF)) << 28) | (((qword)(code)) & 0x0FFFFFFF))

#define __TEXT_WRITE_CHAR (0)
#define TEXT_WRITE_CHAR MKCMD(MODE_TEXT, __TEXT_WRITE_CHAR)

static void __draw_char(int pox, int posy, char ch, int color) {
    qword command[] = {TEXT_WRITE_CHAR, ch, color, pos_x, pos_y};
    send_msg(command, VIDEO_DRIVER_SERVICE, sizeof(command), 20);
}

void putchar(char ch) {
    if (ch == '\r' || ch == '\n') { //制表符
        pos_y ++;
        pos_x = 0;
        __update_pos();
    }
    else if (ch == '\t') {
        pos_x += 4;
        __update_pos();
    }
    else if (ch == '\v') {
        pos_y ++;
        __update_pos();
    }
    else if (ch == '\b') {
        pos_x --;
        __update_pos();
        __draw_char(pos_x, pos_y, ' ', print_color);
    }
    else if(ch == '\f') {
        clrscr();
        pos_x = 0;
        pos_y = 0;
        __update_pos();
    }
    else { //普通字符
        __draw_char(pos_x, pos_y, ch, print_color);
        pos_x ++;
        __update_pos();
    }
    if (pos_y - scroll_line > 0) { //滚动屏幕
        scroll_screen(pos_y - scroll_line);
    }
}

void puts(const char *str) {
    while (*str)
        putchar (*(str ++));
}

static int _vsprintf(char *buffer, const char *format, va_list args) {
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

            switch (*format) {
            case 'l': case 'L': qualifier = QUAL_LONG; format ++; break;
            case 'h': qualifier = QUAL_SHORT; format ++; break;
            }

            switch (*format) {
            case 'l': case 'L': qualifier = QUAL_LONGLONG; format ++; break;
            }

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

            if (print_type == -1) {
                return -1;
            }
            format ++;

            bool has_sign = false;

            int offset = 0;

            char _buffer[120] = {};


            switch (print_type) {
            case PRINT_TY_INT: {
                if (qualifier != QUAL_LONGLONG) {
                    int val = va_arg(args, int);
                    if (val < 0) {
                        has_sign = true;
                        val = -val;
                    }
                    itoa(val, _buffer, 10);
                }
                else {
                    long long val = va_arg(args, long long);
                    if (val < 0) {
                        has_sign = true;
                        val = -val;
                    }
                    lltoa(val, _buffer, 10);
                }
                break;
            }
            case PRINT_TY_UNSIGNED: {
                if (qualifier != QUAL_LONGLONG) {
                    unsigned int val = va_arg(args, unsigned int);
                    uitoa(val, _buffer, 10);
                }
                else {
                    unsigned long long val = va_arg(args, unsigned long long);
                    ulltoa(val, _buffer, 10);
                }
                break;
            }
            case PRINT_TY_FLOAT: {
                double val = va_arg(args, double);
                if (val < 0) {
                    has_sign = true;
                    val = -val;
                }
                dtoa(val, _buffer, precision);
                break;
            }
            case PRINT_TY_EXPONENT: {
                double val = va_arg(args, double);
                if (val < 0) {
                    has_sign = true;
                    val = -val;
                }
                dtoea(val, _buffer, flag & FLAG_UPPER);
                break;
            }
            case PRINT_TY_OCT: {
                if (qualifier != QUAL_LONGLONG) {
                    unsigned int val = va_arg(args, unsigned int);
                    uitoa(val, _buffer, 8);
                }
                else {
                    unsigned long long val = va_arg(args, unsigned long long);
                    ulltoa(val, _buffer, 8);
                }
            }
            case PRINT_TY_HEX: {
                if (qualifier != QUAL_LONGLONG) {
                    unsigned int val = va_arg(args, unsigned int);
                    uitoa(val, _buffer, 16);
                }
                else {
                    unsigned long long val = va_arg(args, unsigned long long);
                    ulltoa(val, _buffer, 16);
                }
                if (flag & FLAG_UPPER) {
                    for (int i = 0 ; i < strlen(_buffer) ; i ++) {
                        if (islower(_buffer[i])) {
                            _buffer[i] = toupper(_buffer[i]);
                        }
                    }
                }
                break;
            }
            case PRINT_TY_CHAR: {
                char ch = (char)va_arg(args, unsigned int);
                *buffer = ch;
                buffer ++;
                offset ++;
                break;
            }
            case PRINT_TY_STRING: {
                char *str = va_arg(args, char*);
                strcpy(buffer, str);
                buffer += strlen(str);
                offset += strlen(str);
                break;
            }
            }

            if (flag & FLAG_SIGN) {
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


            if (flag & FLAG_PREFIX) {
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

            for (int i = strlen(_buffer) + offset ; i < width ; i ++) {
                *buffer = flag & FLAG_FILL_ZERO ? '0' : ' ';
                buffer ++;
            }

            strcpy(buffer, _buffer);
            buffer += strlen(_buffer);
        }
    }

    *buffer = '\0';

    return buffer - original;

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

int printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    static char buffer[256];

    int ret = _vsprintf(buffer, format, args);

    puts(buffer);

    va_end(args);
    return ret;
}

int sprintf(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);

    int ret = _vsprintf(buffer, format, args);

    va_end(args);
    return ret;
}

void disable_cursor_move(void) {
    do_cursor_move = false;
}

void enable_cursor_move(void) {
    do_cursor_move = true;
}