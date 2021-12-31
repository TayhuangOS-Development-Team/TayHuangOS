// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/stage2_main.c
 *
 * Stage2 main function here
 */



#include "drivers/drivers.h"
#include "drivers/devices.h"
#include "printf.h"

#include "intcall.h"
#include "heap.h"
#include "drivers/memory/memory_driver.h"
#include "drivers/disk/disk_driver.h"
#include <string.h>

mem_prode_t prode_mem_result;


void entry(void) {
    ll_init_heap();

    init_devices();
    init_drivers();
    clrscr();

    init_heap();

    // printf ("%#8X ", rd_sector_addr);
    // printf ("%#8X ", alloc(512, false));
    // printf ("%#8X ", alloc(512, false));

    addr_t sector_addr = alloc(512, false);

    APACK(dk, read_sector) pack;
    pack.dst = sector_addr;
    pack.src_low = pack.src_high = 0;
    a_disk_driver.pc_handle(&a_disk_driver, DK_CMD_READ_SECTOR, &pack);

    for (int i = 0 ; i < 16 ; i ++) {
        printf ("%#X0: ", i);
        for (int j = 0 ; j < 16 ; j ++) {
            printf ("%2X ", get_heap_byte(sector_addr + i * 16 + j));
        }
        printf ("\n");
    }

    terminate_drivers();
}