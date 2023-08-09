/**
 * @file baseio.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief primitive io - 实现
 * @version alpha-1.0.0
 * @date 2023-04-08
 *
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#include <basec/baseio.h>
#include <basec/tostring.h>
#include <basec/logger.h>

#include <stdbool.h>
#include <ctype.h>
#include <string.h>

/**
 * @brief 底层printf
 *
 * @param buffer 输出缓冲
 * @param fmt 格式化字符串
 * @param args 参数
 * @return 输出字符数
 */
static int __llprintf(char *buffer, const char *fmt, va_list args) {
    // 标志
    #define FLAG_SIGN 1 // 符号
    #define FLAG_LEFT_ALIGN 2 // 左对齐
    #define FLAG_FILL_ZERO 4 // 填充0
    #define FLAG_PREFIX 8 // 前缀
    #define FLAG_UPPER 16 // 大写

    #define PRINT_TY_INT 0 // 整型
    #define PRINT_TY_UNSIGNED 1 // 无符号整型
    #define PRINT_TY_OCT 2 // 八进制
    #define PRINT_TY_HEX 3 // 十六进制
    #define PRINT_TY_CHAR 4 // 字符
    #define PRINT_TY_STRING 5 // 字符串

    #define QUAL_SHORT 0 // 短
    #define QUAL_NORMAL 1 // 普通
    #define QUAL_LONG 2 // 长

    // 保存原指针
    char *original = buffer;

    // fmt未结束
    while (*fmt) {
        // 非格式化字符
        if (*fmt != '%') {
            // 直接加入
            *buffer = *fmt;
            buffer ++;
            fmt ++;
        }
        else {
            // 格式化
            fmt ++;

            // %%
            if (*fmt == '%') {
                *buffer = '%';
                buffer ++;
                fmt ++;
                continue;
            }

            // 格式化信息
            unsigned char flag = 0;
            unsigned int width = 0;
            unsigned int precision = 0;
            unsigned char print_type = PRINT_TY_INT;
            unsigned char qualifier = QUAL_NORMAL;

            // 特殊标记(+ - 0 #)
            while (true) {
                bool _break = false;
                switch (*fmt) {
                case '+': flag |= FLAG_SIGN; fmt ++; break;
                case '-': flag |= FLAG_LEFT_ALIGN; fmt ++; break;
                case ' ': fmt ++; break;
                case '0': flag |= FLAG_FILL_ZERO; fmt ++; break;
                case '#': flag |= FLAG_PREFIX; fmt ++; break;
                default: _break = true; break; //标记结束
                }
                if (_break) {
                    break;
                }
            }

            // 对齐标记
            // *说明放在可变参数表中
            if (*fmt == '*') {
                width = va_arg(args, int);
                // 负数说明左对齐
                if (width < 0) {
                    width = -width;
                    flag |= FLAG_LEFT_ALIGN;
                }
                fmt ++;
            }
            else {
                // 获取对齐宽度
                while (isdigit(*fmt)) {
                    width = width * 10 + *fmt - '0';
                    fmt ++;
                }
            }

            // 精度标记
            // *说明放在可变参数表中
            if (*fmt == '.') {
                fmt ++;
                if (*fmt == '*') {
                    precision = va_arg(args, int);
                    fmt ++;
                }
                else {
                    // 获取精度
                    while (isdigit(*fmt)) {
                        precision = precision * 10 + *fmt - '0';
                        fmt ++;
                    }
                }
            }

            // 精度拓展标记
            switch (*fmt) {
            case 'l': case 'L': qualifier = QUAL_LONG; fmt ++; break;
            case 'h': qualifier = QUAL_SHORT; fmt ++; break;
            }

            // 类型标记
            switch (*fmt) {
            case 'd': print_type = PRINT_TY_INT; break;
            case 'u': print_type = PRINT_TY_UNSIGNED; break;
            case 'o': print_type = PRINT_TY_OCT; break;
            case 'X': flag |= FLAG_UPPER; // 大写十六进制
            case 'x': print_type = PRINT_TY_HEX; break;
            case 'c': print_type = PRINT_TY_CHAR; break;
            case 's': print_type = PRINT_TY_STRING; break;
            case 'P': flag |= FLAG_UPPER; // 大写指针
            case 'p': flag |= FLAG_FILL_ZERO; flag |= FLAG_PREFIX; width = 16; print_type = PRINT_TY_HEX; break; // 指针
            default: print_type = -1; break;
            }

            // 解析失败
            if (print_type == -1) {
                return -1;
            }
            fmt ++;

            // 符号标记和缓存
            bool has_sign = false;
            int offset = 0;
            char _buffer[120] = {};

            switch (print_type) {
            // 整型
            case PRINT_TY_INT: {
                if (qualifier == QUAL_LONG ||
                    qualifier == QUAL_NORMAL ||
                    qualifier == QUAL_SHORT) {

                    int val = va_arg(args, int);
                    // 小于0则取相反数并设置符号标记
                    if (val < 0) {
                        has_sign = true;
                        val = -val;
                    }
                    itoa(val, _buffer, 10);
                }
                break;
            }
            // 无符号整型
            case PRINT_TY_UNSIGNED: {
                if (qualifier == QUAL_LONG ||
                    qualifier == QUAL_NORMAL ||
                    qualifier == QUAL_SHORT) {

                    unsigned int val = va_arg(args, unsigned int);
                    uitoa(val, _buffer, 10);
                }
                break;
            }
            // 八进制
            case PRINT_TY_OCT: {
                if (qualifier == QUAL_LONG ||
                    qualifier == QUAL_NORMAL ||
                    qualifier == QUAL_SHORT) {

                    unsigned int val = va_arg(args, unsigned int);
                    uitoa(val, _buffer, 8);
                }
                break;
            }
            // 十六进制
            case PRINT_TY_HEX: {
                if (qualifier == QUAL_LONG ||
                    qualifier == QUAL_NORMAL ||
                    qualifier == QUAL_SHORT) {

                    unsigned int val = va_arg(args, unsigned int);
                    uitoa(val, _buffer, 16);

                    // 若大写
                    if (flag & FLAG_UPPER) {
                        char *__buffer = _buffer;
                        while (*__buffer != '\0') {
                            if (islower(*__buffer)) {
                                *__buffer = toupper(*__buffer);
                            }
                            __buffer ++;
                        }
                    }
                }
                break;
            }
            // 字符
            case PRINT_TY_CHAR: {
                char ch = (char)va_arg(args, unsigned int);
                *buffer = ch;
                buffer ++;
                offset ++;
                break;
            }
            // 字符串
            case PRINT_TY_STRING: {
                char *str = va_arg(args, char *);
                strcpy(buffer, str);
                buffer += strlen(str);
                offset += strlen(str);
                break;
            }
            }

            // 符号标记
            if (flag & FLAG_SIGN) {
                // 是否为负数
                if (! has_sign) {
                    // 添加正号
                    *buffer = '+';
                    buffer ++;
                    offset ++;
                }
            }

            // 是否为负数
            if (has_sign) {
                // 添加负号
                *buffer = '-';
                buffer ++;
                offset ++;
            }

            // 前缀
            if (flag & FLAG_PREFIX) {
                // 八进制
                if (print_type == PRINT_TY_OCT) {
                    *buffer = '0';
                    buffer ++;
                }
                else if (print_type == PRINT_TY_HEX) {
                    // 十六进制
                    *buffer = '0';
                    buffer ++;
                    *buffer = flag & FLAG_UPPER ? 'X' : 'x';
                    buffer ++;
                }
            }

            // 填充字符
            char fillch = flag & FLAG_FILL_ZERO ? '0' : ' ';

            // 填充
            for (int i = strlen(_buffer) + offset ; i < width ; i ++) {
                *buffer = fillch;
                buffer ++;
            }

            // 添加到buffer中
            strcpy(buffer, _buffer);
            // 移动指针
            buffer += strlen(_buffer);
        }
    }

    // 结束标记
    *buffer = '\0';

    return buffer - original;

    // 取消类型定义
    #undef PRINT_TY_INT
    #undef PRINT_TY_UNSIGNED
    #undef PRINT_TY_FLOAT
    #undef PRINT_TY_EXPONENT
    #undef PRINT_TY_OCT
    #undef PRINT_TY_HEX
    #undef PRINT_TY_CHAR
    #undef PRINT_TY_STRING

    // 取消标记定义
    #undef FLAG_SIGN
    #undef FLAG_LEFT_ALIGN
    #undef FLAG_FILL_ZERO
    #undef FLAG_PREFIX
    #undef FLAG_UPPER

    // 取消精度定义
    #undef QUAL_SHORT
    #undef QUAL_NORMAL
    #undef QUAL_LONG

    return 0;
}

/**
 * @brief 使用bputs输出
 *
 * @param bputs 输出函数
 * @param fmt 格式化字符串
 * @param args 参数
 * @return 输出字符数
 */
int vbprintf(BaseCPutsFunc bputs, const char *fmt, va_list args) {
    char buffer[512];

    int ret = __llprintf(buffer, fmt, args);
    bputs(buffer);

    return ret;
}

/**
 * @brief 输出到buffer中
 *
 * @param buffer 缓存
 * @param fmt 格式化字符串
 * @param args 参数
 * @return 输出字符数
 */
int vsprintf(char *buffer, const char *fmt, va_list args) {
    return __llprintf(buffer, fmt, args);
}

/**
 * @brief 使用bputs输出
 *
 * @param bputs 输出函数
 * @param fmt 格式化字符串
 * @param ... 参数
 * @return 输出字符数
 */
int bprintf(BaseCPutsFunc bputs, const char *fmt, ...) {
    // 初始化可变参数
    va_list lst;
    va_start(lst, fmt);

    int ret = vbprintf(bputs, fmt, lst);

    va_end(lst);
    return ret;
}

/**
 * @brief 输出到buffer中
 *
 * @param buffer 缓存
 * @param fmt 格式化字符串
 * @param ... 参数
 * @return 输出字符数
 */
int sprintf(char *buffer, const char *fmt, ...)  {
    // 初始化可变参数
    va_list lst;
    va_start(lst, fmt);

    int ret = vsprintf(buffer, fmt, lst);

    va_end(lst);
    return ret;
}

/**
 * @brief 使用bgetchar输入
 *
 * @param bgetchar 输入函数
 * @param fmt 格式化字符串
 * @param args 参数
 * @return 输入字符数
 */
int vbscanf(BaseCGetcharFunc bgetchar, const char *fmt, va_list args) {
    // TODO
    return 0;
}

/**
 * @brief 使用bgetchar输入
 *
 * @param bgetchar 输入函数
 * @param fmt 格式化字符串
 * @param ... 参数
 * @return 输入字符数
 */
int bscanf(BaseCGetcharFunc bgetchar, const char *fmt, ...) {
    // 初始化可变参数
    va_list lst;
    va_start(lst, fmt);

    int ret = vbscanf(bgetchar, fmt, lst);

    va_end(lst);
    return ret;
}