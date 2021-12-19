// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/drivers/devices.c
 *
 * Devices were collected here
 */



#include "devices.h"

device_t vedio_device;

PUBLIC void init_devices(void) {
    vedio_device.id = alloc_id();
    vedio_device.type = DT_VEDIO;
}