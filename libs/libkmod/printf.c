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

static int print_color = 0;
static int tty = -1;

int get_print_color(void) {
    return print_color;
}

void set_print_color(int color) {
    print_color = color;
}

int get_tty(void) {
    return tty;
}

void set_tty(int _tty) {
    tty = _tty;
}

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

void set_active_tty(int _tty) {
    qword command[] = {TTY_SETACTIVE, _tty};
    send_msg(command, TTY_DRIVER_SERVICE, sizeof(command), 20);
}

void change_pos(int x, int y) {
    qword command[] = {TTY_SETPOS, tty, x, y};
    send_msg(command, TTY_DRIVER_SERVICE, sizeof(command), 20);
}

int get_pos_x(void) {
    int x = 0;
    qword command[] = {TTY_GETPOSX, tty};
    sendrecv(command, &x, TTY_DRIVER_SERVICE, sizeof(command), 20);
    return x;
}

int get_pos_y(void) {
    int y = 0;
    qword command[] = {TTY_GETPOSY, tty};
    sendrecv(command, &y, TTY_DRIVER_SERVICE, sizeof(command), 20);
    return y;
}

int get_scroll_line(void) {
    int scroll_line = 0;
    qword command[] = {TTY_GETSCROLLLINE, tty};
    sendrecv(command, &scroll_line, TTY_DRIVER_SERVICE, sizeof(command), 20);
    return scroll_line;
}

void set_scroll_line(int line) {
    qword command[] = {TTY_SETSCROLLLINE, tty, line};
    send_msg(command, TTY_DRIVER_SERVICE, sizeof(command), 20);
}

void clrscr(void) {
    qword command[] = {TTY_CLEAR_SCREEN, tty};
    send_msg(command, TTY_DRIVER_SERVICE, sizeof(command), 20);
}

#define NUM_MAX_CHARACTERS (64)

static qword command[NUM_MAX_CHARACTERS * 2 + 2];
static int write_pos = 0;

void flush_to_screen(void) {
    command[0] = TTY_WRITE_STR;
    command[1] = tty;
    command[2] = write_pos;

    send_msg (command, TTY_DRIVER_SERVICE, sizeof(command), 20);

    write_pos = 0;
}

static void __putchar(char ch) {
    if (write_pos >= NUM_MAX_CHARACTERS) {
        flush_to_screen();
    }
    command[write_pos * 2 + 3] = ch;
    command[write_pos * 2 + 4] = print_color;
    write_pos ++;
}

void putchar(char ch) {
    __putchar(ch);
    flush_to_screen();
}

void puts(const char *str) {
    while (*str)
        __putchar (*(str ++));
    flush_to_screen();
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