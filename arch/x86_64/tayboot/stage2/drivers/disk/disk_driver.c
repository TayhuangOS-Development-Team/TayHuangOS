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
#include "filesystem.h"
#include "filesystems/fs_fat16.h"

PRIVATE struct {
    byte size;
    byte reversed1;
    byte transfer_num;
    byte reversed2;
    addr_t dst_off;
    sreg_t dst_seg;
    dword lba_low;
    dword lba_high;
} disk_address_packet;

PRIVATE struct {
    addr_t file_system;
    dword drive_no;
    enum {
        DK_TY_FLOOPY,
        DK_TY_HARD,
        DK_TY_CD
    }type;
} disk_infos[4];
PRIVATE byte disk_cnt = 0;

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

PRIVATE void _read_sector(pdriver driver, sreg_t segment, addr_t offset, dword lba_high, dword lba_low) {
    disk_address_packet.size = sizeof(disk_address_packet);
    disk_address_packet.transfer_num = 1;
    disk_address_packet.reversed1 = disk_address_packet.reversed2 = 0;
    disk_address_packet.dst_off = offset;
    disk_address_packet.dst_seg = segment;
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
}

PUBLIC void read_sector(pdriver driver, addr_t buffer, dword lba_high, dword lba_low) {
    _read_sector(driver, get_heap_seg(), buffer, lba_high, lba_low);
}

DEF_SUB_CMD(read_sector) {
    PAPACK(dk, read_sector) args = (PAPACK(dk, read_sector))pack;
    read_sector(driver, args->dst, args->src_high, args->src_low);
    return true;
}

PRIVATE addr_t get_file_system(pdriver driver) {
    for (int i = 0 ; i < disk_cnt ; i ++) {
        if (disk_infos[i].drive_no == driver->device->drive_no)
            return disk_infos->file_system;
    }
    return NULL;
}

DEF_SUB_CMD(get_filesystem) {
    *(addr_t*)pack = get_file_system(driver);
    return true;
}

DEF_SUB_CMD(load_file) {
    PAPACK(dk, load_file) args = (PAPACK(dk, load_file))pack;
    addr_t fs = get_file_system(driver);
    if (*(dword*)LDADDR(fs) == 0xD949FA99) {
        fat16_file_t file;
        get_fat16_file_info(args->name, driver, &file);
        if (file.length == -1)
            return false;
        word clus = file.first_clus;
        addr_t offset = args->offset;
        while (clus != 0xFFFF) {
            _read_sector(driver, args->segment, offset, 0, clus + ((pfs_fat16)LDADDR(fs))->data_start);
            offset += 512;
            clus = get_fat16_entry(clus, driver);
        }
        return true;
    }
    return false;
}

DEF_SUB_CMD(init) {
    disk_infos[disk_cnt].type = DK_TY_HARD;
    disk_infos[disk_cnt].drive_no = driver->device->drive_no;
    disk_infos[disk_cnt].file_system = recognize_file_system(driver);
    disk_cnt ++;
    return true;
}

PRIVATE bool process_center(pdriver driver, word cmdty, argpack_t pack) {
    if (driver->state != DS_IDLE || driver->device->type != DT_DISK)
        return false;
    switch (cmdty) {
        case DK_CMD_READ_SECTOR:
            return SUB_CMD(read_sector)(driver, pack);
        case DK_CMD_GET_FILESYSTEM:
            return SUB_CMD(get_filesystem)(driver, pack);
        case DK_CMD_INIT:
            return SUB_CMD(init)(driver, pack);
        case DK_CMD_LOAD_FILE:
            return SUB_CMD(load_file)(driver, pack);
    }
    return false;
}

PRIVATE bool terminate_driver(pdriver driver) {
    if (driver->state == DS_TERMAINATED || driver->device->type != DT_DISK)
        return false;
    driver->state = DS_TERMAINATED;
    for (int i = 0 ; i < disk_cnt ; i ++) {
        if (disk_infos[i].drive_no == driver->device->drive_no) {
            free(disk_infos[i].file_system);
            break;
        }
    }
    return true;
}

PUBLIC void create_disk_driver(pdriver driver) {
    driver->init_handle = (initialize_handle_t)initialize_driver;
    driver->pc_handle = (pc_handle_t)process_center;
    driver->terminate_handle = (terminate_handle_t)terminate_driver;
    driver->state = DS_UNINITIALIZE;
}