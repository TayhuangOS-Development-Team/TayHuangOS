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
#include "../../intcall.h"
#include "../../heap.h"

PRIVATE bool initialize_driver(pdevice device, pdriver driver, id_t id) {
    if (driver->state != DS_UNINITIALIZE || device->type != DT_MEMORY)
        return false;
    driver->dev_ty = DT_MEMORY;
    driver->state = DS_IDLE;
    driver->extensions = NULL;
    driver->id = id;
    device->driver = driver;
    return true;
}

PUBLIC bool prode_memory(pmem_prode result) {
    intargs_t args;
    reg_collect_t in_regs;
    reg_collect_t out_regs;
    addr_t ards_addr;
    in_regs.ebx = 0;
    in_regs.edi = ards_addr = ll_alloc(sizeof(ards_t) * 2);
    in_regs.es = ll_get_heap_seg();
    args.in_regs = &in_regs;
    args.out_regs = &out_regs;
    int cnt = 0;
    do {
        in_regs.eax = 0xE820;
        in_regs.ecx = 20;
        in_regs.edx = 0x534D4150;
        args.int_no = 0x15;
        intcall(&args);
        if (EF_GETCF(out_regs.eflags) != 0) {
            return false;
        }
        ll_cp_from(ards_addr, &(result->ards_list[cnt ++]), 20);
        in_regs.ebx = out_regs.ebx;
    } while (out_regs.ebx);

    result->prode_cnt = cnt;

    return true;
}

DEF_SUB_CMD(init) {
    return false;
}

PRIVATE bool process_center(pdevice device, pdriver driver, word cmdty, argpack_t pack) {
    if (driver->state != DS_IDLE || device->type != DT_MEMORY)
        return false;
    switch (cmdty) {
       case MM_CMD_INIT:
            return SUB_CMD(init)(device, driver, pack);
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