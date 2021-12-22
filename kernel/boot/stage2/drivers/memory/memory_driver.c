// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/drivers/memory/memory_driver.c
 *
 * Memory driver is implemented here
 */



#include "memory_driver.h"

PRIVATE bool initialize_driver(pdevice device, pdriver driver, id_t id) {
    if (driver->state != DS_UNINITIALIZE || device->type != DT_MEMORY)
        return false;
    driver->dev_ty = DT_MEMORY;
    driver->state = DS_IDLE;
    driver->extension = NULL;
    driver->id = id;
    device->driver = driver;
    return true;
}

PRIVATE bool process_center(pdevice device, pdriver driver, word cmdty, argpack_t pack) {
    if (driver->state != DS_IDLE || device->type != DT_MEMORY)
        return false;
    switch (cmdty) {
        default:
            return false;
    }
    return false;
}

PRIVATE bool terminate_driver(pdevice device, pdriver driver) {
    if (driver->state == DS_TERMAINATED || device->type != DT_MEMORY)
        return false;
    driver->state = DS_TERMAINATED;
    return true;
}

PUBLIC void create_memory_driver(pdriver driver) {
    driver->init_handle = (initialize_handle_t)initialize_driver;
    driver->pc_handle = (pc_handle_t)process_center;
    driver->terminate_handle = (terminate_handle_t)terminate_driver;
    driver->state = DS_UNINITIALIZE;
}