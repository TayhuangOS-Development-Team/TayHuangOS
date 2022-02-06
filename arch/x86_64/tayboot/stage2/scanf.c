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
 * arch/x86_64/tayboot/stage2/scanf.c
 *
 * Input functions are implemented here
 */



#include "scanf.h"
#include "drivers/drivers.h"
#include "drivers/keyboard/keyboard_driver.h"
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include "printf.h"
#include "buffer.h"

#define KBF_SZ (512) //击键缓存大小

PRIVATE word key_buffer_front = 0; //击键缓存队列头（入键）
PRIVATE word key_buffer_tail = 0; //击键缓存队列尾（出键）
PRIVATE addr_t key_buffer = NULL; //击键缓存

PRIVATE bool terminate_keyboard(void) { //击键缓存Terminater
    free_buffer(key_buffer);
    key_buffer_front = key_buffer_tail = 0;
    return true;
}

PUBLIC void clear_buffer(void) { //清除缓存
    key_buffer_front = key_buffer_tail = 0;
}

PUBLIC void init_key_buffer(void) { //初始化击键缓存
    key_buffer = alloc_buffer(KBF_SZ, false);
    key_buffer_front = key_buffer_tail = 0;
    register_terminater(terminate_keyboard);
}

PRIVATE bool can_backspace(void) { //能否退格
    return key_buffer_tail < key_buffer_front;
}

PRIVATE void wait_for_enter(void) { //等待enter
    char ch = 0;
    do {
        ch = getkey();
        if (ch == '\0') { //'\0'不处理
            continue;
        }
        if (ch == '\b') { //退格
            if (can_backspace()) { //能退格则退格
                key_buffer_front += (KBF_SZ - 1);
                key_buffer_front %= KBF_SZ;
            }
            continue; //不加入buffer
        }
        set_buffer_byte(key_buffer + (key_buffer_front ++), ch); //加入buffer
        key_buffer_front %= KBF_SZ; //循环队列
    } while (ch != '\r' && ch != '\n'); //若ch为'\n'或'\r'， 结束循环
}

PRIVATE int buffer_next(void) { //取键
    int res = get_buffer_byte(key_buffer + (key_buffer_tail ++));
    key_buffer_tail %= KBF_SZ;
    return res;
}

PRIVATE void wait_for_input(void) { //等待输入
    if (key_buffer_tail >= key_buffer_front) { //缓冲区无字符
        wait_for_enter(); //等待enter键入
    }
}

PUBLIC int getchar(void) {
    wait_for_input(); //等待输入
    return buffer_next(); //取键
}

PUBLIC void backchar(int ch) {
    key_buffer_tail += (KBF_SZ - 1);
    key_buffer_tail %= KBF_SZ; //循环队列
    set_buffer_byte(key_buffer + key_buffer_tail, ch);
}

//是否打印按下的key
PUBLIC bool print_key_typed = true;

PUBLIC int getkey(void) {
    word ch;
    keyboard_driver.pc_handle(&keyboard_driver, KB_CMD_READ_KEY, &ch);
    //如果打印按下的key
    if (print_key_typed) {
        if (isprint(ch&0xFF) || (ch&0xFF) == '\r' || (ch&0xFF) == '\n' ||
                (ch&0xFF) == '\t' || (ch&0xFF) == '\v') { //是否可以直接打印
            putchar(ch&0xFF);
        }
        else if ((ch&0xFF) == '\b' && can_backspace()) { //不能直接打印的退格
            putchar('\b');
        }
    }
    else {
        //不打印则除'\b','\n','\r'外的所有字符都显示为*
        if ((ch&0xFF) == '\b') {
            if (can_backspace())
                putchar('\b');
        }
        else if ((ch&0xFF) == '\n' || (ch&0xFF) == '\r') {
            putchar('\n');
        }
        else {
            putchar('*');
        }
    }
    return ch & 0xFF;
}

#define SCAN_SZ_BYTE (0)
#define SCAN_SZ_WORD (1)
#define SCAN_SZ_DWORD (2)

PRIVATE void _assign(void *ptr, int sz, int value) { //赋值工具
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

PRIVATE void _assignu(void *ptr, int sz, unsigned int value) { //赋值工具
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

//基础vscanf实现
PRIVATE int _vscanf(bkkey_t bk_func, rdkey_t rd_func, const char *format, va_list args) {
    int sum = 0;
    while (*format != 0) { //format不为空字符
        if (*format == '%') { //为格式化字符
            sum ++; //读入值域+1
            int width = -1; //最大读入长度
            int sz = SCAN_SZ_DWORD; //读入大小
            bool flag1 = false; //是否舍弃读入值
            format ++;
            if (*format == '*') { //舍弃
                flag1 = true;
                format ++;
            }
            if (isdigit(*format)) { //为数字, width
                width = 0;
                while (isdigit(*format)) {
                    width = width * 10 + (*format) - '0';
                    format ++;
                }
            }
            if (*format == 'h') { //h: 2Byte
                sz = SCAN_SZ_WORD;
                format ++;
            }
            if (*format == 'h') { //hh: 1Byte
                if (sz == SCAN_SZ_WORD) {
                    sz = SCAN_SZ_BYTE;
                }
                format ++;
            }
            else if (* format == 'l') { //l: 4Byte
                sz = SCAN_SZ_DWORD;
                format ++;
            }
            else if (*format == 'j') { //j: 4Byte
                sz = SCAN_SZ_DWORD;
            }
            else if (*format == 'z') { //z: 4Byte
                sz = SCAN_SZ_DWORD;
            }
            else if (*format == 't') { //t: 4Byte
                sz = SCAN_SZ_DWORD;
            }
            if (*format == 'c') { //character
                if (width == -1) { //默认读入1个字符
                    width = 1;
                }
                if (flag1) { //若舍弃
                    char ch = 0;
                    if (width > 0) {
                        while (width --) ch = rd_func();
                        bk_func(ch); //退回最后一个读入的字符
                    }
                    continue;
                }
                char *ch = va_arg(args, char*);
                while (width --) { //读入
                    *(ch ++) = rd_func();
                }
                bk_func(*(ch - 1)); //退回最后一个读入的字符
            }
            else if (*format == 'd') {
                char buffer[100];
                char ch = rd_func();
                while (isspace(ch)) ch = rd_func();
                int cnt = 0;
                while ((width --) && isdigit(ch)) {  //width不为0且ch为10进制数字继续读入
                    buffer[cnt ++] = ch;
                    ch = rd_func();
                }
                bk_func(ch); //退回最后一个读入的字符
                if (flag1) continue; //舍弃
                buffer[cnt] = 0;
                int value = atoi(buffer);
                _assign(va_arg(args, void*), sz, value); //赋值
            }
            else if (*format == 'u') {
                char buffer[100];
                char ch = rd_func();
                while (isspace(ch)) ch = rd_func();
                int cnt = 0;
                while ((width --) && isdigit(ch)) {  //width不为0且ch为10进制数字继续读入
                    buffer[cnt ++] = ch;
                    ch = rd_func();
                }
                bk_func(ch); //退回最后一个读入的字符
                if (flag1) continue; //舍弃
                buffer[cnt] = 0;
                unsigned int value = atoui(buffer);
                _assignu(va_arg(args, void*), sz, value); //赋值
            }
            else if (*format == 'o') {
                char buffer[100];
                char ch = rd_func();
                while (isspace(ch)) ch = rd_func();
                int cnt = 0;
                while ((width --) && isodigit(ch)) { //width不为0且ch为8进制数字继续读入
                    buffer[cnt ++] = ch;
                    ch = rd_func();
                }
                bk_func(ch); //退回最后一个读入的字符
                if (flag1) continue; //舍弃
                buffer[cnt] = 0;
                int value = atoi_8(buffer);
                _assign(va_arg(args, void*), sz, value); //赋值
            }
            else if (*format == 'x' || *format == 'X') {
                char buffer[100];
                char ch = rd_func();
                while (isspace(ch)) ch = rd_func();
                int cnt = 0;
                while ((width --) && isxdigit(ch)) { //width不为0且ch为16进制数字继续读入
                    buffer[cnt ++] = ch;
                    ch = rd_func();
                }
                bk_func(ch); //退回最后一个读入的字符
                if (flag1) continue; //舍弃
                buffer[cnt] = 0;
                int value = atoi_16(buffer);
                _assign(va_arg(args, void*), sz, value); //赋值
            }
            else if (*format == 's') {
                char *str = va_arg(args, char*);
                char ch = rd_func();
                while (isspace(ch)) ch = rd_func(); //忽略前面的空格
                if (flag1) {
                    while ((width --) && (!isspace(ch))) ch = rd_func(); //width不为0且ch不为空格继续读入
                    bk_func(ch); //退回最后一个读入的字符
                    continue; //舍弃
                }
                while ((width --) && (!isspace(ch))) { //width不为0且ch不为空格继续读入
                    *(str ++) = ch;
                    ch = rd_func();
                }
                bk_func(ch); //退回最后一个读入的字符
                *str = '\0'; //标记字符串结束
            }
            format ++;
        }
        else {
            char ch = rd_func();
            if (ch == *format) { //是否为format所要求
                format ++;
            }
            else {
                break;
            }
        }
    }
    return sum; //返回输入的值域个数
}

PUBLIC int scanf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    int res = _vscanf(backchar, getchar, format, args);

    va_end(args);

    return res;
}

PUBLIC int vscanf(bkkey_t bk_func, rdkey_t rd_func, const char *format, ...) {
    va_list args;
    va_start(args, format);

    int res = _vscanf(bk_func, rd_func, format, args);

    va_end(args);

    return res;
}

//当前文件指针
PRIVATE FILE* current_fp;

//根据当前文件指针实现的__fgetchar
PRIVATE int __fgetchar(void) {
    if (feof(current_fp)) return -1; //到末尾返回eof
    return get_buffer_byte(current_fp->file_buffer + (current_fp->rdpos ++));
}

//根据当前文件指针实现的__fbkchar
PRIVATE void __fbkchar(int ch) {
    current_fp->rdpos --;
}

PUBLIC int fgetc(FILE *file) {
    current_fp = file; //设置当前文件指针
    return __fgetchar();
}

PUBLIC int fscanf(FILE *file, const char *format, ...) {
    va_list args;
    va_start(args, format);

    current_fp = file; //设置当前文件指针

    int res = _vscanf(__fbkchar, __fgetchar, format, args);

    va_end(args);

    return res;
}

PUBLIC void getline(char *str) {
    char ch = getchar();
    while (ch == '\n' || ch == '\r') { //忽略前面的'\n' '\r'
        ch = getchar();
    }
    while (ch != '\n' && ch != '\r') { //不为换行符则读取
        *(str ++) = ch;
        ch = getchar();
    }
    *str = '\0'; //标记字符串结束
}

PUBLIC void fgetline(FILE *file, char *str) {
    char ch = fgetc(file);
    while (ch == '\n' || ch == '\r') { //忽略前面的'\n' '\r'
        ch = fgetc(file);
    }
    while (ch != '\n' && ch != '\r' && (feof(file) == 0)) { //不为换行符则读取
        *(str ++) = ch;
        ch = fgetc(file);
    }
    *str = '\0'; //标记字符串结束
}