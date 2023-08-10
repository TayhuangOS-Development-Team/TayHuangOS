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
#include <basec/logger.h>
#include <tay/types.h>

static byte __HEAP__[HEAP_SIZE];

static byte *__HeapPtr__ = __HEAP__;

static int lastStage = 0;

void *lmalloc(size_t size) {
    void *ret = __HeapPtr__;
    __HeapPtr__ += size;

    size_t usedSize = __HeapPtr__ - __HEAP__;

    if (usedSize * 100 >= HEAP_SIZE && lastStage < 1) {
        lastStage = 1;
        LogWarn("超过1%%的堆已使用!");
    }

    if (usedSize * 10 >= HEAP_SIZE && lastStage < 2) {
        lastStage = 2;
        LogWarn("超过10%%的堆已使用!");
    }

    if (usedSize * 4 >= HEAP_SIZE && lastStage < 3) {
        lastStage = 3;
        LogWarn("超过25%%的堆已使用!");
    }

    if (usedSize * 2 >= HEAP_SIZE && lastStage < 4) {
        lastStage = 4;
        LogWarn("超过50%%的堆已使用!");
    }

    if (usedSize * 4 >= HEAP_SIZE * 3 && lastStage < 5) {
        lastStage = 5;
        LogWarn("超过75%%的堆已使用!");
    }

    return ret;
}

void lfree(void *ptr) {
    // 空实现
}

/**
 * @brief 打印堆情况
 *
 */
void LogHeap(void) {
    size_t usedSize = __HeapPtr__ - __HEAP__;

    LogInfo("----------堆信息----------");
    LogInfo(
        "总大小: %d B(%d KB=%d MB) ; 已使用空间: %d B(%d KB=%d MB)(占比=%d%%)",
        HEAP_SIZE, HEAP_SIZE / 1024, HEAP_SIZE / 1024 / 1024,
        usedSize,  usedSize  / 1024, usedSize  / 1024 / 1024,
        usedSize * 100 / HEAP_SIZE
        );
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