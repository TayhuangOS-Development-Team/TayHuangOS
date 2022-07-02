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
 * clock.c
 * 
 * 时钟中断
 * 
 */



#include <tayhuang/io.h>
#include <tayhuang/ports.h>

#include <intterup/clock/clock.h>

#include <string.h>

#define PIT_FREQUENCY (1193181.6666f)

PUBLIC volatile int ticks = 0;

PUBLIC bool init_pit(float frequency) { //初始化PIT
    if (frequency > PIT_FREQUENCY) //频率过高
        return false;

    int count = (int)(PIT_FREQUENCY / frequency);
    if ((PIT_FREQUENCY - count * frequency) > (frequency / 2))
        count ++;

    if (count >= 65535) return false; //频率过低

    outb(PIT_CHANNEL0, (byte)count); //设置频率
    outb(PIT_CHANNEL0, (byte)(count >> 8)); //设置频率

    return true;
}

PUBLIC short clock_int_handler(int irq, struct intterup_args *regs, bool entered_handler) { //时钟中断
    ticks ++;

    return 0;
}