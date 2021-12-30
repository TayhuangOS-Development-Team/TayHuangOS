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
#include <string.h>

mem_prode_t prode_mem_result;


void entry(void) {
    ll_init_heap();

    init_devices();
    init_drivers();
    clrscr();
    // while (true);
    // printf("233");
    // disp_char('a', 0x0C, 0, 0);

    //while (true);


    init_heap();

    addr_t num = alloc(sizeof(dword), false);
    set_heap_dword(num, 0x114514);
    printf ("%#8X\n", *(dword*)LDADDR(num));
    // //char buffer[100];
    // // itoa(buffer, 100, 10);
    // // puts(buffer);
    // //  printf("aa%#4Xbaa", 233);
    // // printf("%d", get_heap_dword(num));
    // // free(num);

    // // printf("Hello, World!");

    terminate_drivers();
}