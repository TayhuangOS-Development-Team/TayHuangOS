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
 * drivers.c
 *
 * 驱动集合
 *
 */



#include "drivers.h"
#include "video/video_driver.h"
#include "memory/memory_driver.h"
#include "disk/disk_driver.h"
#include "keyboard/keyboard_driver.h"
#include "devices.h"

PUBLIC driver_t video_driver;
PUBLIC driver_t memory_driver;
PUBLIC driver_t a_disk_driver;
PUBLIC driver_t keyboard_driver;

PUBLIC void init_drivers(void) {
    create_video_driver(&video_driver);
    video_driver.init_handle(&video_device, &video_driver, alloc_id());
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
    flag &= video_driver.terminate_handle(&video_driver);
    flag &= memory_driver.terminate_handle(&memory_driver);
    flag &= a_disk_driver.terminate_handle(&a_disk_driver);
    flag &= keyboard_driver.terminate_handle(&keyboard_driver);
    return flag;
}