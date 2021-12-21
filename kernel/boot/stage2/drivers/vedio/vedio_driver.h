// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * Author: Flysong
 *
 * kernel/boot/stage2/drivers/vedio/vedio_driver.h
 *
 * Vedio driver is declared here
 */



#pragma once

#include "../driver.h"

#define VD_CMD_READ_BYTE CMD_TY(0)
#define VD_CMD_WRITE_BYTE CMD_TY(1)
#define VD_CMD_CLRSCR CMD_TY(2)

typedef struct{
    b8 ch;
    b8 color;
}vd_readbyte_out_t, *pvd_readbyte_out;

typedef struct{
    b8 pos_x;
    b8 pos_y;
    pvd_readbyte_out out;
}vd_readbyte_ap_t, *pvd_readbyte_ap_t;


typedef struct{
    b8 pos_x;
    b8 pos_y;
    b8 ch;
    b8 color;
}vd_writebyte_ap_t, *pvd_writebyte_ap_t;

PUBLIC void create_vedio_driver(pdriver driver);