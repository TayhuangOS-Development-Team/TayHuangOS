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
#include "print.h"

#include "intcall.h"
#include "heap.h"

void entry(void) {
    init_devices();
    init_drivers();
    clrscr();

    ll_init_heap();
    addr_t num = ll_alloc(sizeof(dword));
    ll_set_dword(num, 10);
    print_int(ll_get_dword(num), false);
    // print_string("Hello, World!");
}