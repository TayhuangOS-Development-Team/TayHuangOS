// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/drivers/devices.h
 *
 * Devices were collected here
 */



#pragma once

#include "driver.h"

extern device_t vedio_device;
extern device_t memory_device;
extern device_t a_disk_device;
extern device_t keyboard_device;

PUBLIC void init_devices(void);