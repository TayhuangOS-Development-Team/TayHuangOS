// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/drivers/driver.h
 *
 * Driver structures are defined here
 */



#pragma once

#include "../header.h"

typedef enum {
    DT_VEDIO,
    DT_DISK,
    DT_KEYBOARD
}device_ty;

typedef struct {
    device_ty type;
    id_t id;
} device_t, *pdevice;

typedef bool(*initialize_handle_t)(pdevice, void*, id_t);
typedef void *argpack_t;
typedef bool(*pc_handle_t)(pdevice, void*, word, argpack_t);
typedef bool(*terminate_handle_t)(pdevice, void*);

#define CMD_TY(x) (x + 0x100)

typedef enum {
    DS_UNINITIALIZE,
    DS_IDLE,
    DS_BUSY,
    DS_TERMAINATED
}driver_state;

typedef void *drext_t;

typedef struct {
    device_ty dev_ty;
    id_t id;
    driver_state state;
    drext_t extension;
    initialize_handle_t init_handle;
    pc_handle_t pc_handle;
    terminate_handle_t terminate_handle;
} driver_t, *pdriver;

PUBLIC id_t alloc_id(void);