/*
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 * 
 *    Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 * 
 * --------------------------------------------------------------------------------
 * 
 * 作者: Flysong
 * 
 * services.h
 * 
 * 内核模块服务号
 * 
 */



#pragma once

#define API_PID(x) (0x10000 + (x))
#define GDS_PID(x) (0x20000 + (x))

#define SETUP_SERVICE API_PID(0)
#define MM_SERVICE API_PID(1)
#define TASKMAN_SERVICE API_PID(2)
#define VIDEO_DRIVER_SERVICE API_PID(3)

#define GDS_GTTY GDS_PID(0)