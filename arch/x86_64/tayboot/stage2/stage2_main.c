// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage2/stage2_main.c
 *
 * Stage2 main function here
 */



#include "drivers/drivers.h"
#include "drivers/devices.h"

#include "heap.h"
#include "console/console.h"
#include "drivers/disk/disk_driver.h"
#include "drivers/disk/filesystems/fs_fat16.h"
#include "pm/entry.h"
#include "printf.h"
#include "scanf.h"
#include "buffer.h"
#include "file.h"
#include <string.h>

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
    screen_info.height = 25;
    screen_info.width = 80;
    screen_info.current_line = 0;
    screen_info.scroll_line = 18;
    ll_init_buffer();

    init_devices();
    init_drivers();
    clrscr();

    init_buffer();

    init_heap();

    a_disk_driver.pc_handle(&a_disk_driver, DK_CMD_INIT, NULL);

    init_key_buffer();

#ifdef GOTO_OS_AUTO
    go_to_protect_mode();
#else
    enter_console();
#endif

    do_terminate();
}