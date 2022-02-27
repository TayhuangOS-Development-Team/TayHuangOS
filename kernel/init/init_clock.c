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
 * init_clock.c
 *
 * 初始化时钟
 *
 */



#include "init_clock.h"
#include <tayhuang/ports.h>
#include <tayhuang/io.h>

#define PIT_FREQUENCY (1193181.6666f)

PUBLIC bool init_pit(_IN float frequency) {
    if ((frequency > PIT_FREQUENCY) || (frequency < (PIT_FREQUENCY / 65535)))
        return false;

    word count = (word)(PIT_FREQUENCY / frequency);
    if ((PIT_FREQUENCY - count * frequency) > (frequency / 2))
        count ++;

    outw(PIT_CHANNEL0, count);

    return true;
}