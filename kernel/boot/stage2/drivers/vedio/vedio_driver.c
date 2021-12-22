// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/drivers/vedio/vedio_driver.c
 *
 * Vedio driver is implemented here
 */



#include "vedio_driver.h"

PRIVATE bool initialize_driver(pdevice device, pdriver driver, id_t id) {
    if (driver->state != DS_UNINITIALIZE || device->type != DT_VEDIO)
        return false;
    driver->dev_ty = DT_VEDIO;
    driver->state = DS_IDLE;
    driver->extension = NULL;
    driver->id = id;
    device->driver = driver;
    return true;
}

PRIVATE bool process_read_byte_cmd(pdevice device, pdriver driver, argpack_t pack) {
    driver->state = DS_BUSY;
    PAPACK(vd, readbyte) args = (PAPACK(vd, readbyte))pack;
    stgs(0xB800);
    args->out->ch = rdgs8((args->pos_y * 80 + args->pos_x) * 2);
    args->out->color = rdgs8((args->pos_y * 80 + args->pos_x) * 2 + 1);
    driver->state = DS_IDLE;
    return true;
}

void _clrscr(void);
PRIVATE bool process_clrscr_cmd(pdevice device, pdriver driver, argpack_t pack) {
    driver->state = DS_BUSY;
    _clrscr();
    ed_callasm();
    driver->state = DS_IDLE;
    return true;
}

PRIVATE bool process_write_byte_cmd(pdevice device, pdriver driver, argpack_t pack) {
    driver->state = DS_BUSY;
    PAPACK(vd, writebyte) args = (PAPACK(vd, writebyte))pack;
    stgs(0xB800);
    stgs8((args->pos_y * 80 + args->pos_x) * 2, args->ch);
    stgs8((args->pos_y * 80 + args->pos_x) * 2 + 1, args->color);
    driver->state = DS_IDLE;
    return true;
}

PRIVATE bool process_center(pdevice device, pdriver driver, word cmdty, argpack_t pack) {
    if (driver->state != DS_IDLE || device->type != DT_VEDIO)
        return false;
    switch (cmdty) {
    case VD_CMD_READ_BYTE:
        return process_read_byte_cmd(device, driver, pack);
    case VD_CMD_WRITE_BYTE:
        return process_write_byte_cmd(device, driver, pack);
    case VD_CMD_CLRSCR:
        return process_clrscr_cmd(device, driver, pack);
    }
    return false;
}

PRIVATE bool terminate_driver(pdevice device, pdriver driver) {
    if (driver->state == DS_TERMAINATED || device->type != DT_VEDIO)
        return false;
    driver->state = DS_TERMAINATED;
    return true;
}

PUBLIC void create_vedio_driver(pdriver driver) {
    driver->init_handle = (initialize_handle_t)initialize_driver;
    driver->pc_handle = (pc_handle_t)process_center;
    driver->terminate_handle = (terminate_handle_t)terminate_driver;
    driver->state = DS_UNINITIALIZE;
}