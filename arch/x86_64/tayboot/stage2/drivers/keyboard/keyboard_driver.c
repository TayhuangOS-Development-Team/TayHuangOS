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
 * arch/x86_64/tayboot/stage2/drivers/keyboard/keyboard_driver.c
 *
 * 键盘驱动
 *
 */



#include "keyboard_driver.h"
#include "../../intcall.h"
#include "../../printf.h"

PRIVATE bool initialize_driver(pdevice device, pdriver driver, id_t id) {
    if (driver->state != DS_UNINITIALIZE || device->type != DT_KEYBOARD)
        return false;
    driver->dev_ty = DT_KEYBOARD;
    driver->state = DS_IDLE;
    driver->extensions = NULL;
    driver->id = id;
    driver->device = device;
    device->driver = driver;
    return true;
}

DEF_SUB_CMD(read_key) {
    intargs_t args;
    reg_collect_t in_regs;
    reg_collect_t out_regs;
    in_regs.eax = 0; //ah = 0
    args.in_regs = &in_regs;
    args.out_regs = &out_regs;
    args.int_no = 0x16;
    intcall(&args);
    *(word*)pack = out_regs.eax & 0xFFFF;
    return true;
}

PRIVATE bool process_center(pdriver driver, word cmdty, argpack_t pack) {
    if (driver->state != DS_IDLE || driver->device->type != DT_KEYBOARD)
        return false;
    switch (cmdty) {
    case KB_CMD_READ_KEY:
        return SUB_CMD(read_key)(driver, pack);
    }
    return false;
}

PRIVATE bool terminate_driver(pdriver driver) {
    if (driver->state == DS_TERMAINATED || driver->device->type != DT_KEYBOARD)
        return false;
    driver->state = DS_TERMAINATED;
    return true;
}

PUBLIC void create_keyboard_driver(pdriver driver) {
    driver->init_handle = (initialize_handle_t)initialize_driver;
    driver->pc_handle = (pc_handle_t)process_center;
    driver->terminate_handle = (terminate_handle_t)terminate_driver;
    driver->state = DS_UNINITIALIZE;
}