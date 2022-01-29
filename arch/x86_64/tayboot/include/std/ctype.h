// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/include/std/ctype.h
 *
 * Standard lib ctype.h header here
 */



#pragma once

#define _macro_isspace(ch) (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch == '\v' || ch == '\f')
#define _macro_isupper(ch) (ch >= 'A' && ch <= 'Z')
#define _macro_islower(ch) (ch >= 'a' && ch <= 'z')
#define _macro_isalpha(ch) (_macro_isupper(ch) || _macro_islower(ch))
#define _macro_isdigit(ch) (ch >= '0' && ch <= '9')
#define _macro_isalnum(ch) (_macro_isalpha(ch) || _macro_isdigit(ch))
#define _macro_isblank(ch) (ch == ' ' || ch == '\t')
#define _macro_iscntrl(ch) ((ch >= 0x00 && ch <= 0x1f) || ch == 0x7f)
#define _macro_isprint(ch) (!_macro_iscntrl(ch))
#define _macro_isgraph(ch) (_macro_isprint(ch))
#define _macro_ispunct(ch) (_macro_isprint(ch) && (! _macro_isalnum(ch)))
#define _macro_isxdigit(ch) (_macro_isalnum(ch) || ((ch >= 'a' && ch <= 'F') || (ch >= 'A' && ch <= 'F')))
#define _macro_isodigit(ch) (ch >= '0' && ch <= '7')
#define _macro_tolower(ch) (_macro_isupper(ch) ? (ch - 'A' + 'a') : (ch))
#define _macro_toupper(ch) (_macro_islower(ch) ? (ch - 'a' + 'A') : (ch))

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