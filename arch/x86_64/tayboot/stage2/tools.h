/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage2/tools.h
 *
 * 定义工具函数
 *
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

PUBLIC dword get_clock_time(void); //获取tick
PUBLIC void get_time(struct time_t *tm); //获取时间
PUBLIC void get_date(struct date_t *dt); //获取日期
PUBLIC byte bcd2num4(byte bcd); //bcd转数字
PUBLIC byte bcd2num8(byte bcd); //bcd转数字
PUBLIC int random(dword seed, int min, int max); //随机数生成器, result 属于 [min, max)
PUBLIC char escape(const char *sentence); //低级转义器