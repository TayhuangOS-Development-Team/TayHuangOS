// SPDX-License-Identifier: GPL-2.0-only
/* -------------------------------*-TayhuangOS-*-----------------------------------
 *
 *   Copyright (C) 2022, 2022 TayhuangOS Development Team - All Rights Reserved
 *
 * --------------------------------------------------------------------------------
 *
 * 作者: Flysong
 *
 * arch/x86_64/tayboot/stage2/pm/vesa.h
 *
 * Vesa functions are declared here
 */



#pragma once

#include "../header.h"
#include "../intcall.h"

PUBLIC bool support_vesa(void);
PUBLIC void* enable_graphic(void);