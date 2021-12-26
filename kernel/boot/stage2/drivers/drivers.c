// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/drivers/drivers.c
 *
 * Drivers were collected here
 */



#include "drivers.h"
#include "./vedio/vedio_driver.h"
#include "./memory/memory_driver.h"
#include "devices.h"

driver_t vedio_driver;
driver_t memory_driver;

PUBLIC void init_drivers(void) {
    create_vedio_driver(&vedio_driver);
    vedio_driver.init_handle(&vedio_device, &vedio_driver, alloc_id());
    create_memory_driver(&memory_driver);
    memory_driver.init_handle(&memory_device, &memory_driver, alloc_id());
}

PUBLIC void terminate_drivers(void) {
    vedio_driver.terminate_handle(&vedio_device, &vedio_driver);
    memory_driver.terminate_handle(&memory_device, &memory_driver);
}