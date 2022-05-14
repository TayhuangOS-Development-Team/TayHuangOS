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
 * timer.c
 * 
 * 时间相关
 * 
 */



#include "timer.h"
#include "../ipc/ipc.h"

int get_ticks(void) {
    int ticks;
    long long pack[] = {0x00, (long long)&ticks};
    send_msg(pack, 0x10000, sizeof(pack));
    return ticks;
}