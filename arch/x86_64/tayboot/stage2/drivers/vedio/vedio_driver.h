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

#define VD_CMD_READ_BYTE CMD_TY(0) // PAPACK(vd, readbyte)
#define VD_CMD_WRITE_BYTE CMD_TY(1) // PAPACK(vd, writebyte)
#define VD_CMD_CLRSCR CMD_TY(2) // NULL

typedef struct{
    b8 ch;
    b8 color;
}DOPACK(vd, readbyte);

typedef struct{
    b8 pos_x;
    b8 pos_y;
    POPACK(vd,readbyte) out;
}DAPACK(vd, readbyte);


typedef struct{
    b8 pos_x;
    b8 pos_y;
    b8 ch;
    b8 color;
}DAPACK(vd, writebyte);

PUBLIC void create_vedio_driver(pdriver driver);