/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * ctype.h
 *
 * 标准库 ctype.h 头文件
 *
 */



#pragma once

/*
 * ctype函数的宏实现
 * 因为是利用宏, 所以可能会引发问题 如 __isspace(getchar())
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
int isspace(int ch);
int isupper(int ch);
int islower(int ch);
int isalpha(int ch);
int isdigit(int ch);
int isalnum(int ch);
int isblank(int ch);
int iscntrl(int ch);
int isprint(int ch);
int isgraph(int ch);
int ispunct(int ch);
int isxdigit(int ch);
int isodigit(int ch);
int tolower(int ch);
int toupper(int ch);