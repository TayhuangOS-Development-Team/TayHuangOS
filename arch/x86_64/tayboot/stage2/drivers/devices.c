// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage2/drivers/devices.c
 *
 * Devices were collected here
 */



#include "devices.h"

PUBLIC device_t vedio_device;
PUBLIC device_t memory_device;
PUBLIC device_t a_disk_device;
PUBLIC device_t keyboard_device;

PUBLIC void init_devices(void) {
    vedio_device.id = alloc_id();
    vedio_device.type = DT_VEDIO;
    vedio_device.drive_no = 0;
    memory_device.id = alloc_id();
    memory_device.type = DT_MEMORY;
    memory_device.drive_no = 0;
    a_disk_device.id = alloc_id();
    a_disk_device.type = DT_DISK;
    a_disk_device.drive_no = 0x80;
    keyboard_device.id = alloc_id();
    keyboard_device.type = DT_KEYBOARD;
    keyboard_device.drive_no = 0;
}