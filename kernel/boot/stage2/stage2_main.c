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
#include "drivers/memory/memory_driver.h"

mem_prode_t prode_result;

void entry(void) {
    init_devices();
    init_drivers();
    clrscr();

    ll_init_heap();
    // addr_t num = ll_alloc(sizeof(dword));
    // ll_set_dword(num, 10);
    // print_int(ll_get_dword(num), false);
    // print_string("Hello, World!");
    if (prode_memory(&prode_result)) {
        dword mem_cnt = 0;
        for (int i = 0 ; i < prode_result.prode_cnt ; i ++) {
            print_int_hex(prode_result.ards_list[i].base_address_high, true);
            print_int_hex(prode_result.ards_list[i].base_address_low, false);
            print_char(' ');
            print_int_hex(prode_result.ards_list[i].length_high, true);
            print_int_hex(prode_result.ards_list[i].length_low, false);
            print_char(' ');
            print_int_hex(prode_result.ards_list[i].type, true);
            print_return();
            mem_cnt = max(mem_cnt, prode_result.ards_list[i].base_address_low + prode_result.ards_list[i].length_low);
        }
        print_int_hex(mem_cnt, true);
    }
}