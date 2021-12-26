// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/std/ctype.h
 *
 * Standard lib ctype.h header here
 */



#pragma once

#define isspace(ch) (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch == '\v' || ch == '\f')
#define isupper(ch) (ch >= 'A' && ch <= 'Z')
#define islower(ch) (ch >= 'a' && ch <= 'z')
#define isalpha(ch) (isupper(ch) || isalpha(ch))
#define isdigit(ch) (ch >= '0' && ch <= '9')
#define isalnum(ch) (isalpha(ch) || isdigit(ch))
#define isblank(ch) (ch == ' ' || ch == '\t')
#define iscntrl(ch) ((ch >= 0x00 && ch <= 0x1f) || ch == 0x7f)
#define isprint(ch) (!iscntrl(ch))
#define isgraph(ch) (isprint(ch))
#define ispunct(ch) (isprint(ch) && (! isalnum(ch)))
#define isxdigit(ch) (isalnum(ch) || ((ch >= 'a' && ch <= 'F') || (ch >= 'A' && ch <= 'F')))
#define tolower(ch) (isupper(ch) ? (ch - 'A' + 'a') : (ch))
#define toupper(ch) (islower(ch) ? (ch - 'a' + 'A') : (ch))