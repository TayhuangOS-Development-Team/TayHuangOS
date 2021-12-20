// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/std/stdarg.h
 *
 * stdarg header here
 */



#pragma once

typedef char *va_list;
typedef va_list *pva_list;

#define __calcsz__(x) ((sizeof(x) + sizeof(int) - 1) & (~(sizeof(int) - 1)))

#define va_start(lst, start)	((lst) = (va_list)&start + __calcsz__(start))
#define va_arg(lst, ty) (*((type*)((va_list)((lst) = (void*)((va_list)(lst) + __calcsz__(ty))) - __calcsz__(ty))))
#define va_end(lst)