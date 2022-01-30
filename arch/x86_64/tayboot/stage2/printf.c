// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong Beanflame
 *
 * arch/x86_64/tayboot/stage2/print.c
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
#include "buffer.h"

PUBLIC struct screen_info_t screen_info;

PRIVATE byte print_color = 0x0F;

PUBLIC byte get_print_color(void) {
    return print_color;
}

PUBLIC void set_print_color(byte color) {
    print_color = color;
}

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
    if (ch == '\r' || ch == '\n') {
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
            disp_char(' ', print_color, -- print_x, print_y);
        }
    }
    else if(ch == '\f') {
        clrscr();
        print_x = 0;
        print_y = 0;
    }
    else {
        disp_char(ch, print_color, print_x ++, print_y);
    }
    if (print_x == screen_info.width) {
        print_x = 0;
        print_y ++;
    }
    if (print_y - screen_info.scroll_line > 0) {
        scroll_screen(print_y - screen_info.scroll_line);
        print_y = screen_info.scroll_line;
    }
    set_cursor_pos(print_x, print_y);
}

PUBLIC void puts(const char *str) {
    while (*str != 0) {
        putchar(*(str ++));
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
    vedio_driver.pc_handle(&vedio_driver, VD_CMD_CLRSCR, NULL);
    print_x = 0;
    print_y = 0;
    screen_info.current_line = 0;
}

//基础vsprintf实现
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
            if (*format == 'd') { //整数
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
            else if (*format == 'u') { //无符号整数
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
            else if (*format == 'c') { //字符
                char ch = va_arg(args, char);
                *(buffer ++) = ch;
            }
            else if (*format == 's') { //字符串
                const char* str;
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
            else if (*format == 'x') { //16进制数
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
            else if (*format == 'X') { //大写的十六进制数
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

    char buffer[256];
    int res = _vsprintf(buffer, format, args);
    puts(buffer);

    va_end(args);

    return res;
}

PUBLIC void scroll_screen(int offset) {
    intargs_t args;
    reg_collect_t in_regs;
    reg_collect_t out_regs;
    args.in_regs = &in_regs;
    args.out_regs = &out_regs;
    args.int_no = 0x10;
    in_regs.eax = MKWORD(((offset < 0) ? 0x7 : 0x6), abs(offset)); //是负数下滚，否则上滚
    in_regs.ebx = 0;
    in_regs.ecx = MKWORD(0, 0); //设定区域
    in_regs.edx = MKWORD(screen_info.height - 1, screen_info.width - 1); //设定区域
    intcall(&args);
    screen_info.current_line += offset;
}

//当前文件指针
PRIVATE FILE *current_file;

//利用当前文件指针制作的简单putc
PUBLIC void __fputc(FILE *file, char ch) {
    if (current_file->buffer_len < current_file->wrpos) { //如果file buffer不够大
        //就扩大它呀!
        int newlen = current_file->buffer_len * 41 / 25; //41/25约为黄金风格比例
        addr_t new_buffer = alloc_buffer(newlen, false);
        cp_buffer_to_buffer(current_file->file_buffer, new_buffer, current_file->buffer_len); //把旧内容拷贝过来
        current_file->buffer_len = newlen;
        free_buffer (current_file->file_buffer); //清除原来的缓存
        current_file->file_buffer = new_buffer;
    }
    set_buffer_byte(current_file->file_buffer + (current_file->wrpos ++), ch); //写入文件buffer
}

PUBLIC void fputc(FILE *file, char ch) {
    current_file = file; //设置当前文件指针
    return __fputc(file, ch);
}

PUBLIC void fputs(FILE *file, const char *str) {
    while (*str != 0) {
        fputc(file, *(str ++));
    }
}

PUBLIC int fprintf(FILE *file, const char *format, ...) {
    va_list args;
    va_start(args, format);

    char buffer[256];
    int res = _vsprintf(buffer, format, args);
    fputs(file, buffer);

    va_end(args);

    return res;
}