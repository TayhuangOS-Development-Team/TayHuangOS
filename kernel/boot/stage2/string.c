// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/string.c
 *
 * String functions are implemented here
 */



#include "string.h"

PUBLIC int strcmp(const char *str1, const char *str2) {
    int res = 0;
    while (!(res = (unsigned char) *(str1++) - (unsigned char) *(str2++)) && *str1);

    if (res < 0)
        return -1;
    else if (res > 0)
        return 1;
    return 0;
}

PUBLIC int strlen(const char *str) {
    int res = 0;
    while (*(str ++)) res ++;
    return res;
}