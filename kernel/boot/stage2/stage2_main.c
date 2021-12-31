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

    addr_t fs = recognize_file_system(&a_disk_driver);
    if (*((dword*)LDADDR(fs)) == 0xD949FA99) {
        print_fat16_file_system(fs);
    }

    free(fs);


    terminate_drivers();
}