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
#include "drivers/disk/filesystems/fs_fat16.h"
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


    // for (int i = 0 ; i < 16 ; i ++) {
    //     printf ("%#X0: ", i);
    //     for (int j = 0 ; j < 16 ; j ++) {
    //         printf ("%2X ", get_heap_byte(sector_addr + i * 16 + j));
    //     }
    //     printf ("\n");
    // }

    // printf ("%#X", chk_is_fat16(sector_addr) ? 0x114514 : 0x1919810);

    a_disk_driver.pc_handle(&a_disk_driver, DK_CMD_INIT, NULL);

    // addr_t fs;
    // a_disk_driver.pc_handle(&a_disk_driver, DK_CMD_GET_FILESYSTEM, &fs);
    // if (*((dword*)LDADDR(fs)) == 0xD949FA99) {
    //     print_fat16_file_system(fs);
    // }

    // for (int i = 0 ; i < 40 ; i ++) {
    //     printf("%#4X: %#4X,     ", i, get_fat16_entry(i, &a_disk_driver));
    // }

    APACK(dk, load_file) args;
    args.name = "SPLASHESTXT";
    args.segment = 0x8000;
    args.offset = 0x0000;

    if (! a_disk_driver.pc_handle(&a_disk_driver, DK_CMD_LOAD_FILE, &args)) {
        printf ("Error!");
    }
    else {
        for (int i  = 0 ; i < 512 ; i ++) {
            stfs(0x8000);
            putchar (rdfs8(i));
        }
    }

    terminate_drivers();
}