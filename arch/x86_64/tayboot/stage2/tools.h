// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/tools.h
 *
 * Tool functions are declared here
 */



#pragma once

#include "header.h"

struct time_t {
    int second;
    int minute;
    int hour;
    bool isdst;
};

struct date_t {
    int day;
    int month;
    int year;
};

PUBLIC dword get_clock_time(void);
PUBLIC void get_time(struct time_t *tm);
PUBLIC void get_date(struct date_t *dt);
PUBLIC byte bcd2num4(byte bcd);
PUBLIC byte bcd2num8(byte bcd);
PUBLIC int random(dword seed, int min, int max);