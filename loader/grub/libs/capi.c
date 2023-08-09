/**
 * @file capi.c
 * @author theflysong (song_of_the_fly@163.com)
 * @brief CAPI
 * @version alpha-1.0.0
 * @date 2022-12-31
 *
 * @copyright Copyright (c) 2022 TayhuangOS Development Team
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#include <libs/capi.h>
#include <tay/types.h>

static byte __HEAP__[HEAP_SIZE];

static byte *__HeapPtr__ = __HEAP__;

void *lmalloc(size_t size) {
    void *ret = __HeapPtr__;
    __HeapPtr__ += size;
    return ret;
}

void lfree(void *ptr) {
    // 空实现
}

static word printPosX = 0;
static word printPosY = 0;
static const word charPerLine = 80;
static word *VIDEO_MEMORY = 0xB8000;
static const byte printColor = 0x0F;

static void LPutRawChar(char ch) {
    VIDEO_MEMORY[printPosX + printPosY * 80] = (((printColor & 0xFF) << 8) + (ch & 0xFF));
}

void lputchar(char ch) {
    switch (ch) {
        case '\r':
        case '\n': {
            printPosX = 0;
            printPosY ++;
            break;
        }
        case '\t': {
            printPosX += 4;
            break;
        }
        case '\v': {
            printPosY ++;
            break;
        }
        case '\f': {
            printPosX = 0;
            printPosY = 0;
            // clrscr();
            break;
        }
        case '\b': {
            printPosX --;
            LPutRawChar(' ');
            break;
        }
        default: {
            LPutRawChar(ch);
            printPosX ++;
        }
    }

    if (printPosX >= charPerLine) { //自动换行
        printPosX -= charPerLine;
        printPosY ++;
    }
}

void lputs(const char *str) {
    while (*str != '\0') {
        lputchar(*str);
        str ++;
    }
}