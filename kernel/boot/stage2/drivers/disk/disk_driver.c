// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/drivers/disk/disk_driver.c
 *
 * Disk driver is implemented here
 */



#include "disk_driver.h"
#include "../../heap.h"
#include "../../intcall.h"
#include "../../printf.h"

PRIVATE addr_t rd_sector_addr = NULL;
PRIVATE struct {
    byte size;
    byte reversed1;
    byte transfer_num;
    byte reversed2;
    addr_t dst_off;
    sreg_t dst_seg;
    dword lba_high;
    dword lba_low;
} disk_address_packet;


PRIVATE bool initialize_driver(pdevice device, pdriver driver, id_t id) {
    if (driver->state != DS_UNINITIALIZE || device->type != DT_DISK)
        return false;
    driver->dev_ty = DT_DISK;
    driver->state = DS_IDLE;
    driver->extensions = NULL;
    driver->id = id;
    device->driver = driver;
    driver->device = device;
    return true;
}

PRIVATE bool process_center(pdriver driver, word cmdty, argpack_t pack) {
    if (driver->state != DS_IDLE || driver->device->type != DT_DISK)
        return false;
    switch (cmdty) {
        default:
            return false;
    }
    return false;
}

/*PRIVATE*/ void read_sector(pdriver driver, dword lba_high, dword lba_low) {
    if (rd_sector_addr == NULL) {
        rd_sector_addr = alloc(512, false);
    }
    disk_address_packet.size = sizeof(disk_address_packet);
    disk_address_packet.transfer_num = 1;
    disk_address_packet.reversed1 = disk_address_packet.reversed2 = 0;
    disk_address_packet.dst_off = rd_sector_addr;
    disk_address_packet.dst_seg = get_heap_seg();
    disk_address_packet.lba_high = lba_high;
    disk_address_packet.lba_low = lba_low;
    intargs_t args;
    reg_collect_t in_regs;
    reg_collect_t out_regs;
    in_regs.eax = MKDWORD(0, MKWORD(0x42, 0));
    in_regs.edx = MKDWORD(0, MKWORD(0, driver->device->drive_no));
    in_regs.esi = MKDWORD(0, &disk_address_packet);
    args.in_regs = &in_regs;
    args.out_regs = &out_regs;
    args.int_no = 0x13;
    intcall(&args);
    for (int i = 0 ; i < 8 ; i ++) {
        printf ("%d0: ", i);
        for (int j = 0 ; j < 16 ; j ++) {
            printf ("%2X ", get_heap_byte(rd_sector_addr + i * 16 + j));
        }
        printf ("\n");
    }
}

PRIVATE bool terminate_driver(pdriver driver) {
    if (driver->state == DS_TERMAINATED || driver->device->type != DT_DISK)
        return false;
    driver->state = DS_TERMAINATED;
    return true;
}

PUBLIC void create_disk_driver(pdriver driver) {
    driver->init_handle = (initialize_handle_t)initialize_driver;
    driver->pc_handle = (pc_handle_t)process_center;
    driver->terminate_handle = (terminate_handle_t)terminate_driver;
    driver->state = DS_UNINITIALIZE;
}