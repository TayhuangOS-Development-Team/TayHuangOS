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
    DT_KEYBOARD,
    DT_MEMORY
}device_ty;

typedef struct {
    device_ty type;
    id_t id;
    void *driver;
} device_t, *pdevice;

typedef bool(*initialize_handle_t)(pdevice, void*, id_t);
typedef void *argpack_t;
typedef bool(*pc_handle_t)(pdevice, void*, word, argpack_t);
typedef bool(*terminate_handle_t)(pdevice, void*);

#define CMD_TY(x) (x + 0x100)
#define APACK(suf, name) suf##_##name##_ap_t
#define OPACK(suf, name) suf##_##name##_out_t
#define POPACK(suf, name) p##suf##_##name##_out
#define PAPACK(suf, name) p##suf##_##name##_ap
#define DAPACK(suf, name) APACK(suf, name), *PAPACK(suf, name)
#define DOPACK(suf, name) OPACK(suf, name), *POPACK(suf, name)
#define SUB_CMD(cmdname) process_##cmdname##_cmd
#define DEF_SUB_CMD(cmdname) PRIVATE bool SUB_CMD(cmdname)(pdevice device, pdriver driver, argpack_t pack)

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
    drext_t *extensions;
    initialize_handle_t init_handle;
    pc_handle_t pc_handle;
    terminate_handle_t terminate_handle;
} driver_t, *pdriver;

PUBLIC id_t alloc_id(void);