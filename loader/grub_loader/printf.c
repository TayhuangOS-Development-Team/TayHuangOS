/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * printf.c
 *
 * printf
 *
 */



#include "printf.h"

void write_char(char ch, int color, int posx, int posy) {
    int pos = posx + posy * 80;
    *(((short*)VIDEO_ADDRESS) + pos) = (((color & 0xFF) << 8) + (ch & 0xFF));
}

void write_str(const char *str, int color, int posx, int posy) {
    int pos = posx + posy * 80;
    while (*str != '\0') {
        char ch = *str;
        *(((short*)VIDEO_ADDRESS) + pos) = (((color & 0xFF) << 8) + (ch & 0xFF));
        pos ++;
        str ++;
    }
}