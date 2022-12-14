/**
 * @file ctype.h
 * @author theflysong (song_of_the_fly@163.com)
 * @brief 标准库头文件 ctype.h
 * @version alpha-1.0.0
 * @date 2022-10-22
 * 
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 * 
 */



#pragma once

#include <__header.h>

__C_HEADER_START;

/*
 * ctype函数的宏实现
 * 因为是利用宏, 所以可能会引发问题 如__isaplha(getchar())
 * 不推荐直接使用
 * 仅用于辅助函数实现
*/
#define __isspace(ch) (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch == '\v' || ch == '\f')
#define __isupper(ch) (ch >= 'A' && ch <= 'Z')
#define __islower(ch) (ch >= 'a' && ch <= 'z')
#define __isalpha(ch) (__isupper(ch) || __islower(ch))
#define __isdigit(ch) (ch >= '0' && ch <= '9')
#define __isalnum(ch) (__isalpha(ch) || __isdigit(ch))
#define __isblank(ch) (ch == ' ' || ch == '\t')
#define __iscntrl(ch) ((ch >= 0x00 && ch <= 0x1f) || ch == 0x7f)
#define __isprint(ch) (!__iscntrl(ch))
#define __isgraph(ch) (__isprint(ch))
#define __ispunct(ch) (__isprint(ch) && (! __isalnum(ch)))
#define __isxdigit(ch) (__isalnum(ch) || ((ch >= 'a' && ch <= 'F') || (ch >= 'A' && ch <= 'F')))
#define __isodigit(ch) (ch >= '0' && ch <= '7')
#define __tolower(ch) (__isupper(ch) ? (ch - 'A' + 'a') : (ch))
#define __toupper(ch) (__islower(ch) ? (ch - 'a' + 'A') : (ch))

/* ctype函数 */
EXPORT int isspace(int ch);
EXPORT int isupper(int ch);
EXPORT int islower(int ch);
EXPORT int isalpha(int ch);
EXPORT int isdigit(int ch);
EXPORT int isalnum(int ch);
EXPORT int isblank(int ch);
EXPORT int iscntrl(int ch);
EXPORT int isprint(int ch);
EXPORT int isgraph(int ch);
EXPORT int ispunct(int ch);
EXPORT int isxdigit(int ch);
EXPORT int isodigit(int ch);
EXPORT int tolower(int ch);
EXPORT int toupper(int ch);

__C_HEADER_END;