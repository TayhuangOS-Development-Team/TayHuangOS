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
 * driver.h
 *
 * 驱动结构
 *
 */



#pragma once

#include "../header.h"

typedef enum {
    DT_VIDEO,
    DT_DISK,
    DT_KEYBOARD,
    DT_MEMORY
}device_ty; //设备类型

typedef struct {
    device_ty type;
    id_t id;
    dword drive_no;
    void *driver;
} device_t, *pdevice; //设备

typedef bool(*initialize_handle_t)(pdevice, void*, id_t);
typedef void *argpack_t;
typedef bool(*pc_handle_t)(void*, word, argpack_t);
typedef bool(*terminate_handle_t)(void*);

#define CMD_TY(x) (x + 0x100)
#define APACK(suf, name) suf##_##name##_ap_t
#define OPACK(suf, name) suf##_##name##_out_t
#define POPACK(suf, name) p##suf##_##name##_out
#define PAPACK(suf, name) p##suf##_##name##_ap
#define DAPACK(suf, name) APACK(suf, name), *PAPACK(suf, name)
#define DOPACK(suf, name) OPACK(suf, name), *POPACK(suf, name)
#define SUB_CMD(cmdname) process_##cmdname##_cmd
#define DEF_SUB_CMD(cmdname) PRIVATE bool SUB_CMD(cmdname)(pdriver driver, argpack_t pack)

typedef enum {
    DS_UNINITIALIZE,
    DS_IDLE,
    DS_BUSY,
    DS_TERMAINATED
}driver_state; //驱动状态

typedef struct {
    device_ty dev_ty;
    id_t id;
    driver_state state;
    initialize_handle_t init_handle;
    pc_handle_t pc_handle;
    terminate_handle_t terminate_handle;
    pdevice device;
} driver_t, *pdriver; //驱动信息

PUBLIC id_t alloc_id(void);