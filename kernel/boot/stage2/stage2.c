// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/stage2.c
 *
 * Stage2 main here
 */



#include "header.h"
#include "print.h"
#include "memory.h"
#include "disk.h"
#include "string.h"


void entry(void) {
    clrscr();
    initfsinfo();
    // readsector(0, 0, BASE_OF_DATA, 0x0000);
    // stes(BASE_OF_DATA);
    // for (short i = 0 ; i < 10 ; i ++) {
    //     for (short j = 0 ; j < 16 ; j ++) {
    //         dispbytehex(rdes8(0x0000 + i * 16 + j), 0x0C, j * 3, i, false);
    //     }
    // }


    //dispstr("hello, os world!", 0x0C, 0, 0);

    printshorthex(findfile("KERNEL  BIN"), true);
    // int a = rdfs();
    // stfs(0x2000);
    // byte data1 = rdfs8(0);
    // stfs(a);
    // printbytehex(data1, true);

    //printbytehex(rdes8(1), true);
    //printchar(' ');
    // for (int i = 0 ; i < 4 ; i ++) {
    // }
    // printfsinfo();
}