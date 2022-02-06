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
 * devices.h
 *
 * 设备集合
 *
 */



#pragma once

#include "driver.h"

extern device_t video_device;
extern device_t memory_device;
extern device_t a_disk_device;
extern device_t keyboard_device;

PUBLIC void init_devices(void);