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

mem_prode_t prode_mem_result;

void entry(void) {
    ll_init_heap();

    init_devices();
    init_drivers();
    clrscr();

    init_heap();

    addr_t num = alloc(sizeof(dword), false);
    set_heap_dword(num, 2);
    print_int_hex(get_heap_dword(num), true);
    free(num);
    // addr_t num = ll_alloc(sizeof(dword));
    // ll_set_dword(num, 10);
    // print_int(ll_get_dword(num), false);
    // print_string("Hello, World!");
    // dword mem_sz;
    // ((pdriver)memory_device.driver)->pc_handle(&memory_device, memory_device.driver, MM_CMD_GET_MEM_SIZE, &mem_sz);
    // if (((pdriver)memory_device.driver)->pc_handle(&memory_device, memory_device.driver, MM_CMD_PRODE, &prode_mem_result)) {
    //     print_string("   Base Address         Length          Type   ");
    //     print_return();
    //     for (int i = 0 ; i < prode_mem_result.prode_cnt ; i ++) {
    //         print_int_hex(prode_mem_result.ards_list[i].base_address_high, true);
    //         print_int_hex(prode_mem_result.ards_list[i].base_address_low, false);
    //         print_char(' ');
    //         print_int_hex(prode_mem_result.ards_list[i].length_high, true);
    //         print_int_hex(prode_mem_result.ards_list[i].length_low, false);
    //         print_char(' ');
    //         print_int_hex(prode_mem_result.ards_list[i].type, true);
    //         print_return();
    //     }
    //     print_string("Memory Size: ");
    //     print_int_hex(mem_sz, true);
    //     print_string("B, ");
    //     print_int_hex(mem_sz / 1024, true);
    //     print_string("KB, ");
    //     print_int_hex(mem_sz / 1024 / 1024, true);
    //     print_string("MB");
    // }



    terminate_drivers();
}