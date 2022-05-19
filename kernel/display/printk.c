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
 * printk.c
 *
 * 内核printk
 *
 */



#include "printk.h"
#include "video.h"
#include "../kheap.h"
#include <tool/tostring.h>
#include <stdarg.h>
#include <tayhuang/io.h>
#include <ctype.h>
#include <string.h>

#define OUTPUT_BUFFER_SIZE (2000 + 80 * 250)

PRIVATE word output_buffer[OUTPUT_BUFFER_SIZE];
PRIVATE int current_pos = 0;
PRIVATE int start_pos = 0;
PRIVATE int print_color = 0;
PRIVATE int scroll_line = 0;
PRIVATE bool do_cursor_move = true;

PRIVATE void set_cursor_pos(int pos) {
    outb(0x3D4, 0x0F);
	outb(0x3D5, (byte) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (byte) ((pos >> 8) & 0xFF));
}

PUBLIC int get_print_color(void) {
    return print_color;
}

PUBLIC void set_print_color(int color) {
    print_color = color;
}

PUBLIC void change_pos(int x, int y) {
    current_pos = y * VIDEO_INFO.width + x;
    if (do_cursor_move)
        set_cursor_pos(current_pos - start_pos);
}

PUBLIC void change_cursor_pos(int x, int y) {
    int pos = y * VIDEO_INFO.width + x;
    set_cursor_pos(pos - start_pos);
}

PUBLIC int get_pos_x(void) {
    return current_pos % VIDEO_INFO.width;
}

PUBLIC int get_pos_y(void) {
    return current_pos / VIDEO_INFO.width;
}

PUBLIC int get_scroll_line(void) {
    return scroll_line;
}

PUBLIC void set_scroll_line(int line) {
    scroll_line = line;
}

PUBLIC void clrscr(void) {
    memset(VIDEO_INFO.framebuffer, 0, VIDEO_INFO.width * VIDEO_INFO.height * 2);
    memset(output_buffer, 0, 0);
    start_pos = 0;
    current_pos = 0;
}

PUBLIC void flush_to_screen(void) {
    memcpy(VIDEO_INFO.framebuffer, &output_buffer[start_pos], VIDEO_INFO.width * VIDEO_INFO.height * 2);
    if (do_cursor_move)
        set_cursor_pos(current_pos - start_pos);
}

PUBLIC void scroll_screen(int lines) {
    start_pos += lines * VIDEO_INFO.width;
    flush_to_screen();
}

PUBLIC void putchar(char ch) {
    if (ch == '\r' || ch == '\n') { //制表符
        int line = current_pos / VIDEO_INFO.width;
        current_pos = (line + 1) * VIDEO_INFO.width;
        if (do_cursor_move)
            output_buffer[current_pos] = MKWORD(0x0F, LOWBYTE(output_buffer[current_pos]));
        flush_to_screen();
    }
    else if (ch == '\t') {
        current_pos += 4;
        if (do_cursor_move)
            output_buffer[current_pos] = MKWORD(0x0F, LOWBYTE(output_buffer[current_pos]));
        flush_to_screen();
    }
    else if (ch == '\v') {
        current_pos += VIDEO_INFO.width;
        if (do_cursor_move)
            output_buffer[current_pos] = MKWORD(0x0F, LOWBYTE(output_buffer[current_pos]));
        flush_to_screen();
    }
    else if (ch == '\b') {
        if (do_cursor_move)
            output_buffer[current_pos - 1] = MKWORD(0x0F, ' ');
        current_pos --;
        flush_to_screen();
    }
    else if(ch == '\f') {
        clrscr();
        current_pos = 0;
        start_pos = 0;
        if (do_cursor_move)
            output_buffer[current_pos] = MKWORD(0x0F, LOWBYTE(output_buffer[current_pos]));
        flush_to_screen();
    }
    else { //普通字符
        output_buffer[current_pos ++] = MKWORD(print_color, ch);
        if (do_cursor_move)
            output_buffer[current_pos] = MKWORD(0x0F, LOWBYTE(output_buffer[current_pos]));
    }
    int y = (current_pos - start_pos) / VIDEO_INFO.width;
    if (y - scroll_line > 0) { //滚动屏幕
        scroll_screen(y - scroll_line);
    }
}

PUBLIC void puts(const char *str) {
    while (*str)
        putchar (*(str ++));
}

PRIVATE int _vsprintk(char *buffer, const char *format, va_list args) {
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

PUBLIC int printk(const char *format, ...) {
    va_list args;
    va_start(args, format);

    static char buffer[256];

    int ret = _vsprintk(buffer, format, args);

    puts(buffer);

    va_end(args);
    return ret;
}

PUBLIC int sprintk(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);

    int ret = _vsprintk(buffer, format, args);

    va_end(args);
    return ret;
}

void panic(const char *format, ...) {
    dis_int(); //关中断(阻止进程切换)

    va_list args;
    va_start(args, format);

    static char buffer[256];

    _vsprintk(buffer, format, args);

    puts(buffer);
    flush_to_screen();

    va_end(args);

    while (true);
    asmv ("ud2"); //不应该被执行
}

PUBLIC void disable_cursor_move(void) {
    do_cursor_move = false;
}

PUBLIC void enable_cursor_move(void) {
    do_cursor_move = true;
}