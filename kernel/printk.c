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
 * printk.c
 *
 * printk
 *
 */



#include <printk.h>

#include <tool/tostring.h>

#include <string.h>
#include <ctype.h>

PRIVATE void *video_address = NULL;
PRIVATE int screen_width = 0;
PRIVATE int screen_height = 0;

//初始化视频信息
PUBLIC void init_video(void *video_addr, int width, int height) {
    video_address = video_addr;
    screen_width = width;
    screen_height = height;
}

//写生字符(串)
PUBLIC void write_char(char ch, int color, int posx, int posy) {
    int pos = posx + posy * 80;
    *(((short*)video_address) + pos) = (((color & 0xFF) << 8) + (ch & 0xFF));
}

PUBLIC void write_str(const char *str, int color, int posx, int posy) {
    int pos = posx + posy * 80;
    while (*str != '\0') {
        char ch = *str;
        *(((short*)video_address) + pos) = (((color & 0xFF) << 8) + (ch & 0xFF));
        pos ++;
        str ++;
    }
}

//变量定义
PRIVATE int print_color = 0x0F;
PRIVATE short print_x = 0;
PRIVATE short print_y = 0;

//获得输出颜色
PUBLIC int get_print_color(void) {
    return print_color;
}

//设置输出颜色
PUBLIC void set_print_color(int color) {
    print_color = color;
}

PUBLIC void putchar(char ch) {
    //特殊字符
    if (ch == '\r' || ch == '\n') { //换行
        print_x = 0;
        print_y ++;
    }
    else if (ch == '\t') { //缩进
        print_x += 4;
    }
    else if (ch == '\v') { //下一行(x不变)
        print_y ++;
    }
    else if (ch == '\b') { //退格
        if (print_x > 0) {
            write_char(' ', print_color, -- print_x, print_y);
        }
    }
    else if(ch == '\f') { //换页
        clrscr();
        print_x = 0;
        print_y = 0;
    }
    else { //普通字符
        write_char(ch, print_color, print_x ++, print_y);
    }
    if (print_x >= screen_width) { //自动换行
        print_x -= screen_width;
        print_y ++;
    }
}

PUBLIC void puts(const char *str) { // 打印字符串
    while (*str != '\0') { //没遇到\0就不停止
        putchar(*str);
        str ++;
    }
}

PUBLIC void change_pos(int x, int y) { //改变输出位置
    print_x = x;
    print_y = y;
}

//获得输出位置
PUBLIC int get_pos_x(void) {
    return print_x;
}

PUBLIC int get_pos_y(void) {
    return print_y;
}

PUBLIC void clrscr(void) {
    //通过画空格实现清屏
    for (int i = 0 ; i < screen_width ; i ++) {
        for (int j = 0 ; j < screen_height ; j ++) {
            write_char(' ', print_color, i, j);
        }
    }
    //重置输出位置
    print_x = 0;
    print_y = 0;
}

//基础vsprintk实现
PUBLIC int vsprintk(char *buffer, const char *format, va_list args) {
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

//输出内容到buffer中
PUBLIC int sprintk(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);

    int res = vsprintk(buffer, format, args);

    va_end(args);

    return res;
}

//输出内容到屏幕中
PUBLIC int printk(const char *format, ...) {
    va_list args;
    va_start(args, format);

    char buffer[256];
    int res = vsprintk(buffer, format, args);
    puts(buffer);

    va_end(args);

    return res;
}