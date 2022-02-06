/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2021, 2021 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage2/drivers/video/video_driver.h
 *
 * 视频驱动
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

PUBLIC void create_video_driver(pdriver driver);