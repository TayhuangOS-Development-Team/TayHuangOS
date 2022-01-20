// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
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

#include "heap.h"
#include "pm/entry.h"
#include "console/console.h"
#include "drivers/disk/disk_driver.h"
#include "printf.h"
#include "scanf.h"
#include "buffer.h"

#define MAX_TERMINATER_NUM (32)
PRIVATE terminater_t TERMINATE_LIST[MAX_TERMINATER_NUM];
PRIVATE byte temlst_sz = 0;

PUBLIC bool register_terminater(terminater_t terminater) {
    if (temlst_sz > MAX_TERMINATER_NUM) return false;
    TERMINATE_LIST[temlst_sz ++] = terminater;
    return true;
}

PUBLIC bool do_terminate(void) {
    bool flag = true;
    for(byte i = 0 ; i < temlst_sz ; i ++) {
        flag &= TERMINATE_LIST[i]();
    }
    return flag;
}

void entry(void) {
    ll_init_buffer();

    init_devices();
    init_drivers();
    clrscr();

    init_buffer();

    init_heap();

    a_disk_driver.pc_handle(&a_disk_driver, DK_CMD_INIT, NULL);

    init_key_buffer();

    enter_console();
    //go_to_protect_mode();

    // int* ptr = malloc(sizeof(int));
    // *ptr = 2;
    // printf ("%d\n", *ptr);
    // *ptr = 4;
    // printf ("%d\n", *ptr);
    // free(ptr);

    while (true);

    do_terminate();
}