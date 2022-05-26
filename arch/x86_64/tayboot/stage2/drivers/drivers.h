/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * drivers.h
 *
 * 驱动集合
 *
 */



#pragma once

#include "driver.h"

extern driver_t video_driver; //视频驱动
extern driver_t memory_driver; //内存驱动
extern driver_t a_disk_driver; //硬盘驱动
extern driver_t keyboard_driver; //键盘驱动

PUBLIC void init_drivers(void);
PUBLIC bool terminate_drivers(void);