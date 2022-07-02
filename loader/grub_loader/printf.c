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
 * printf
 *
 */



#include <printf.h>

#include <ctype.h>
#include <string.h>

#include <tool/tostring.h>

//写生字符(串)
PUBLIC void write_char(char ch, int color, int posx, int posy) {
    int pos = posx + posy * 80;
    *(((short*)VIDEO_ADDRESS) + pos) = (((color & 0xFF) << 8) + (ch & 0xFF));
}

PUBLIC  void write_str(const char *str, int color, int posx, int posy) {
    int pos = posx + posy * 80;
    while (*str != '\0') {
        char ch = *str;
        *(((short*)VIDEO_ADDRESS) + pos) = (((color & 0xFF) << 8) + (ch & 0xFF));
        pos ++;
        str ++;
    }
}

PRIVATE int print_color = 0x0F;
PRIVATE short print_x = 0;
PRIVATE short print_y = 0;

PUBLIC int get_print_color(void) {
    return print_color;
}

PUBLIC void set_print_color(int color) {
    print_color = color;
}

PUBLIC void putchar(char ch) {
    if (ch == '\r' || ch == '\n') { //特殊字符
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
            write_char(' ', print_color, -- print_x, print_y);
        }
    }
    else if(ch == '\f') {
        clrscr();
        print_x = 0;
        print_y = 0;
    }
    else { //普通字符
        write_char(ch, print_color, print_x ++, print_y);
    }
    if (print_x >= SCREEN_WIDTH) { //自动换行
        print_x -= SCREEN_WIDTH;
        print_y ++;
    }
}

PUBLIC void puts(const char *str) { // 打印字符串
    while (*str != '\0') {
        putchar(*str);
        str ++;
    }
}

PUBLIC void change_pos(int x, int y) { //改变输出位置
    print_x = x;
    print_y = y;
}

PUBLIC int get_pos_x(void) {
    return print_x;
}

PUBLIC int get_pos_y(void) {
    return print_y;
}

PUBLIC void clrscr(void) {
    //通过画空格实现清屏
    for (int i = 0 ; i < SCREEN_WIDTH ; i ++) {
        for (int j = 0 ; j < SCREEN_HEIGHT ; j ++) {
            write_char(' ', print_color, i, j);
        }
    }
    print_x = 0;
    print_y = 0;
}

//基础vsprintf实现
PUBLIC int vsprintf(char *buffer, const char *format, va_list args) {
    #define FLAG_SIGN 1
    #define FLAG_LEFT_ALIGN 2
    #define FLAG_FILL_ZERO 4
    #define FLAG_PREFIX 8
    #define FLAG_UPPER 16

    #define PRINT_TY_INT 0
    #define PRINT_TY_UNSIGNED 1
    #define PRINT_TY_OCT 2
    #define PRINT_TY_HEX 3
    #define PRINT_TY_CHAR 4
    #define PRINT_TY_STRING 5

    #define QUAL_SHORT 0
    #define QUAL_NORMAL 1
    #define QUAL_LONG 2

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
            case 'd': print_type = PRINT_TY_INT; break;
            case 'u': print_type = PRINT_TY_UNSIGNED; break;
            case 'o': print_type = PRINT_TY_OCT; break;
            case 'X': flag |= FLAG_UPPER;
            case 'x': print_type = PRINT_TY_HEX; break;
            case 'c': print_type = PRINT_TY_CHAR; break;
            case 's': print_type = PRINT_TY_STRING; break;
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
                if (qualifier == QUAL_LONG ||
                    qualifier == QUAL_NORMAL ||
                    qualifier == QUAL_SHORT) {

                    int val = va_arg(args, int);
                    if (val < 0) {
                        has_sign = true;
                        val = -val;
                    }
                    itoa(val, _buffer, 10);
                }
                break;
            }
            case PRINT_TY_UNSIGNED: {
                if (qualifier == QUAL_LONG ||
                    qualifier == QUAL_NORMAL ||
                    qualifier == QUAL_SHORT) {

                    unsigned int val = va_arg(args, unsigned int);
                    uitoa(val, _buffer, 10);
                }
                break;
            }
            case PRINT_TY_OCT: {
                if (qualifier == QUAL_LONG ||
                    qualifier == QUAL_NORMAL ||
                    qualifier == QUAL_SHORT) {

                    unsigned int val = va_arg(args, unsigned int);
                    uitoa(val, _buffer, 8);
                }
                break;
            }
            case PRINT_TY_HEX: {
                if (qualifier == QUAL_LONG ||
                    qualifier == QUAL_NORMAL ||
                    qualifier == QUAL_SHORT) {

                    unsigned int val = va_arg(args, unsigned int);
                    uitoa(val, _buffer, 16);
                    
                    if (flag & FLAG_UPPER) {
                        for (int i = 0 ; i < strlen(_buffer) ; i ++) {
                            if (islower(_buffer[i])) {
                                _buffer[i] = toupper(_buffer[i]);
                            }
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
    
    #undef QUAL_SHORT
    #undef QUAL_NORMAL
    #undef QUAL_LONG
}

//输出内容到buffer中
PUBLIC int sprintf(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);

    int res = vsprintf(buffer, format, args);

    va_end(args);

    return res;
}

//输出内容到屏幕中
PUBLIC int printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    char buffer[256];
    int res = vsprintf(buffer, format, args);
    puts(buffer);

    va_end(args);

    return res;
}