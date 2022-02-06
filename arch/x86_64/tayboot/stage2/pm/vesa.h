/* 
 * SPDX-License-Identifier: GPL-3.0-only
 * -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage2/pm/vesa.h
 *
 * vesa函数
 *
 */



#pragma once

#include "../header.h"
#include "../intcall.h"

PUBLIC bool support_vesa(void); //是否支持vesa
PUBLIC void *enable_graphic(void); //启用图形化