// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage2/drivers/drivers.c
 *
 * Drivers were collected here
 */



#include "drivers.h"
#include "vedio/vedio_driver.h"
#include "memory/memory_driver.h"
#include "disk/disk_driver.h"
#include "keyboard/keyboard_driver.h"
#include "devices.h"

PUBLIC driver_t vedio_driver;
PUBLIC driver_t memory_driver;
PUBLIC driver_t a_disk_driver;
PUBLIC driver_t keyboard_driver;

PUBLIC void init_drivers(void) {
    create_vedio_driver(&vedio_driver);
    vedio_driver.init_handle(&vedio_device, &vedio_driver, alloc_id());
    create_memory_driver(&memory_driver);
    memory_driver.init_handle(&memory_device, &memory_driver, alloc_id());
    create_disk_driver(&a_disk_driver);
    a_disk_driver.init_handle(&a_disk_device, &a_disk_driver, alloc_id());
    create_keyboard_driver(&keyboard_driver);
    keyboard_driver.init_handle(&keyboard_device, &keyboard_driver, alloc_id());
    register_terminater(terminate_drivers);
}

PUBLIC bool terminate_drivers(void) {
    bool flag = true;
    flag &= vedio_driver.terminate_handle(&vedio_driver);
    flag &= memory_driver.terminate_handle(&memory_driver);
    flag &= a_disk_driver.terminate_handle(&a_disk_driver);
    flag &= keyboard_driver.terminate_handle(&keyboard_driver);
    return flag;
}